#pragma once

class Volume;
struct VolumeNode;

#include "primitives/point.h"
#include "spectra/spectrum.h"
#include "textures/texture.h"
#include "samplers/sampler.h"

#include <vector>

struct VolumeNode {
public:
	std::vector<const Volume*> volumes;
	VolumeNode* prev;

	VolumeNode() : prev(nullptr) { }
};

class Volume {
private:
	const Texture* m_color;
	const Texture* m_density;
	const Texture* m_aniso;

	float phase(float aniso, float cosTheta) const;
	float tr(const Point3& from, const Vector3& direction, float distance, Sampler* sampler) const;

public:
	Volume() = delete;
	Volume(const Texture* color, const Texture* density, const Texture* aniso)
		: m_color(color), m_density(density), m_aniso(aniso) { }

	Spectrum phase(const Point3& p, float cosTheta) const;
	Spectrum samplePhase(const Point3& p, const Vector3& wo, Vector3* wi, Sampler* sampler) const;
	float transmittance(const Point3& from, const Point3& to, Sampler* sampler) const;
	float sampleVolume(const Point3& p, const Vector3& direction, bool* sampledVolume, Point3* sampledPoint,
		Sampler* sampler) const;
};

