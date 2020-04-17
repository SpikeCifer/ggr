#include "intersection/intersection.h"
#include "tools/util.h"

#include <iostream>
#include <limits>

Intersection::Intersection() : hit(false), t(std::numeric_limits<float>::max()), light(nullptr) { }
Intersection::Intersection(const Intersection& other) : Intersection(other.hit, other.t,
	other.wo, other.intersectionPoint, other.material, other.light) { }
Intersection::Intersection(bool hit, float t, const Vector3& wo, const SurfacePoint& intersectionPoint,
	const std::shared_ptr<Material>& material, const LightEntity* light) : hit(hit), t(t), wo(wo),
	intersectionPoint(intersectionPoint), material(material), light(light) { }
Intersection::Intersection(bool hit, float t, const Vector3& wo, const SurfacePoint& intersectionPoint) :
	hit(hit), t(t), wo(wo), intersectionPoint(intersectionPoint) { }
Intersection::Intersection(float t, const Vector3& wo, const SurfacePoint& intersectionPoint) :
	Intersection(true, t, wo, intersectionPoint) { }

void Intersection::print() const {
	std::cout << "Hit: ";
	if (hit) {
		std::cout << "True";
	}
	else {
		std::cout << "False";
	}
	std::cout << std::endl;

	std::cout << "t: " << t << std::endl;
	std::cout << "wo: ";
	wo.print();
	intersectionPoint.print();
}

void Intersection::calculateScreenDifferentials(const Ray& ray) {
	if (!hit || !ray.isCameraRay) {
		return;
	}
	
	Vector3 dpdx;
	Vector3 dpdy;

	float NoP = -intersectionPoint.geometricNormal.dot(intersectionPoint.point);
	float NoD = intersectionPoint.geometricNormal.dot(ray.dxDirection);
	if (NoD != 0.0f) {
		float NoO = intersectionPoint.geometricNormal.dot(ray.dxOrigin);
		dpdx = ray.dxOrigin + ray.dxDirection * (-(NoO + NoP) / NoD);
	}

	NoD = intersectionPoint.geometricNormal.dot(ray.dyDirection);
	if (NoD != 0.0f) {
		float NoO = intersectionPoint.geometricNormal.dot(ray.dyOrigin);
		dpdy = ray.dyOrigin + ray.dyDirection * (-(NoO + NoP) / NoD);
	}

	float xx = 0.0f;
	float yx = 0.0f;
	float xy = 0.0f;
	float yy = 0.0f;

	float dux = 0.0f;
	float duy = 0.0f;
	float dvx = 0.0f;
	float dvy = 0.0f;

	if (!util::equals(intersectionPoint.geometricNormal.z, 0.0f)) {
		xx = dpdx.x - intersectionPoint.point.x;
		yx = dpdx.y - intersectionPoint.point.y;
		xy = dpdy.x - intersectionPoint.point.x;
		yy = dpdy.y - intersectionPoint.point.y;

		dux = intersectionPoint.dpdu.x;
		duy = intersectionPoint.dpdu.y;
		dvx = intersectionPoint.dpdv.x;
		dvy = intersectionPoint.dpdv.y;
	}
	else if (!util::equals(intersectionPoint.geometricNormal.y, 0.0f)) {
		xx = dpdx.x - intersectionPoint.point.x;
		yx = dpdx.z - intersectionPoint.point.z;
		xy = dpdy.x - intersectionPoint.point.x;
		yy = dpdy.z - intersectionPoint.point.z;

		dux = intersectionPoint.dpdu.x;
		duy = intersectionPoint.dpdu.z;
		dvx = intersectionPoint.dpdv.x;
		dvy = intersectionPoint.dpdv.z;
	}
	else {
		xx = dpdx.y - intersectionPoint.point.y;
		yx = dpdx.z - intersectionPoint.point.z;
		xy = dpdy.y - intersectionPoint.point.y;
		yy = dpdy.z - intersectionPoint.point.z;

		dux = intersectionPoint.dpdu.y;
		duy = intersectionPoint.dpdu.z;
		dvx = intersectionPoint.dpdv.y;
		dvy = intersectionPoint.dpdv.z;
	}

	float det = dux * dvy - duy * dvx;
	if (det != 0.0f) {
		float oneOverDet = 1.0f / det;
		intersectionPoint.dUVdx.x = (xx * dvy - yx * dvx) * oneOverDet;
		intersectionPoint.dUVdx.y = (yx * dux - xx * duy) * oneOverDet;
		intersectionPoint.dUVdy.x = (xy * dvy - yy * dvx) * oneOverDet;
		intersectionPoint.dUVdy.y = (yy * dux - xy * duy) * oneOverDet;
	}
}

bool Intersection::operator==(const Intersection& other) const {
	return intersectionPoint == other.intersectionPoint
		&& hit == other.hit
		&& t == other.t
		&& wo == other.wo;
}

