#pragma once

class MatteMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "shaders/diffuse-shader.h"

class MatteMaterial : public Material {
private:
	const Texture* m_diffuseColor;
	
public:
	MatteMaterial() = delete;
	MatteMaterial(const MatteMaterial& other) = delete;
	MatteMaterial(const Texture* diffuseColor) : Material(), m_diffuseColor(diffuseColor) { }

	Shader* createShader(const SurfacePoint& point, const Vector3& wo, StackAllocator& alloc) const {
		return alloc.construct<DiffuseShader>(DiffuseShader(point.shadingNormal, point.tangent, point.bitangent,
			m_diffuseColor->sample(point)));
	}
};

