#pragma once

class Material;

#include "bsdfs/bsdf.h"
#include "intersection/surface-point.h"

class Material {
public:
	Material();
	Material(const Material& other);

	virtual BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const = 0;
};

