#include "materials/metal-material.h"
#include "tools/util.h"
#include "shaders/specular-shader.h"
#include "shaders/mf-reflection-shader.h"

MetalMaterial::MetalMaterial(const Texture* color, const Texture* roughness)
	: Material(), m_color(color), m_roughness(roughness) { }

Shader* MetalMaterial::createShader(const SurfacePoint& point, const Vector3& wo, StackAllocator& alloc) const {
	float roughness = util::clamp(m_roughness->sample(point).value(), 0.0f, 1.0f);
	roughness *= roughness;
	if (util::equals(roughness, 0.0f)) {
		return alloc.construct<SpecularShader>(SpecularShader(point.shadingNormal, point.tangent,
		point.bitangent, m_color->sample(point)));
	}
	return alloc.construct<MFReflectionShader>(MFReflectionShader(point.shadingNormal, point.tangent,
		point.bitangent, m_color->sample(point), roughness));
}
