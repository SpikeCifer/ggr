#include "materials/emission-material.h"
#include "bsdfs/emission-bxdf.h"

EmissionMaterial::EmissionMaterial() : Material(), m_intensity(0.0f) { }
EmissionMaterial::EmissionMaterial(const EmissionMaterial& other) : Material(other),
	m_emission(other.m_emission), m_intensity(other.m_intensity) { }
EmissionMaterial::EmissionMaterial(const std::shared_ptr<Texture> emission, float intensity) :
	Material(), m_emission(emission), m_intensity(intensity) { }

BSDF EmissionMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point);
	std::unique_ptr<BXDF> emission(new EmissionBXDF(m_emission->sample(point) * m_intensity));
	bsdf.addBXDF(emission);
	return bsdf;
}

