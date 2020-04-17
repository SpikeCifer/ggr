# ggr - A physically based offline renderer
![alt text](https://raw.githubusercontent.com/loukoum/ggr/master/renders/the-one-with-the-cat.png)

ggr is a physically based offline renderer written in C++. Everything was written from scratch
including the primitives used like vector classes. The purpose of the project is to create a renderer
that can create realistic pictures, indistinguishable from ones taken from a real camera and to
create practice grounds to write and test more complicated methods for computer graphics.

## Features
### Integrators
- **Direct lighting integrator**: A simple integrator that calculates only direct light. This integrator
does not support any reflections/refractions nor global illumination.

- **Path Tracer**: This integrator computes light paths (radiance paths) that start from the camera. It
uses the surface's BRDF to increment the path with each bounce and Russian roulette to decide when to
end it. This integrator supports every material and can estimate both direct and indirect lighting.
The path tracer is the most used integrator to calculate global illumination in most offline renderers.

### Materials
ggr currently supports six materials but they are enough to emulate most of the real life with proper
parametrization. An all purpose (uber) material is currently on the works that will be added to the future.
All of the materials can be parametrized not only with values but also with texture maps like a roughness
texture map. ggr does not currently support bumping although this feature is on the works.
The list of the current materials are:
- **Matte material**: A fully diffuse material that reflects light uniformly over the hemisphere. These materials
are also known as Lambertian.
- **Mirrors**: A material that completely reflects like using a fully specular delta-function BRDF.
- **Transparent** : A material that refracts and reflects light using a specular delta-function BRDF. This
material is sampled using the surfaces Fresnel factor to minimize variance.
- **Metal**: A material described by the Cook-Torrance microfacet model. This material reflects light
depending on the surface's roughness. The roughness parameter controls the surface's microfacet distribution
and thus changes the way that light reflects of off it. To minimize variance, this material is sampled using
importance sampling by first sampling a microfacet normal and then converting the pdf.
- **Glass**: A material described by the refraction microfacet model by B. Walter et al. This material can
create rough glass-like surfaces like iced windows. To minimize variance, the sampling direction (reflection
or refraction) is first chosen depending the surface's Fresnel factor and then the same scheme of importance
sampling as the metal material is used.
- **Plastic Material**: A combination of a diffuse BRDF and a microfacet reflection BRDF that is controlled
by a parameter.Although it is called plastic, with proper parametrization this material can describe many
real life materials like marble, fined wood, ceramic surfaces or leather.

### Geometry
ggr only supports triangle meshes. This decision was taken to have a consistent ray-intersection test.
There are currently three pre-written shapes: Cubes, spheres and planes. A mesh loader is currently on
the works that will add the ability to load 3D models created by modeling programs.

### Lighting
The only realistic light model is the area light so ggr only supports area lights, or surfaces that have
an emission material on them. ggr also supports spherical skyboxes. These are lights that are infinitely
far away from the scene and surround the scene like a sphere.

### Sampling and filtering
ggr tries the best to minimize variance when possible. As described in some of the materials, whenever possible,
ggr will use methods of sampling that reduce variance like importance sampling. Another part that uses such sampling
techniques is the direct lighting sampling. ggr will sample direct lighting using multiple importance sampling. This is
done by calculate weights and then using importance sampling both on the surface and the light distribution of the scene
to obtain the result. This result is then weighted using the balanced heuristic. Although in these parts ggr will try
to provide better sampling, it currently does not support a good way to create low discrepancy samples over any dimension
and only uses a stohastic sampler (a sampler that just creates random numbers) to create samples. In the future, more
sophisticated methods of creating samples will be added like a stratified sampler and Sobol sampler (sequences).
For filtering, ggr currently supports a simple box filter, a Gaussian filter and a Lanczos filter but will add more
in the future.

### Cameras
The only camera models that ggr currently supports are the typical projective camera models, perspective and orthographic
cameras. Both of these models support a depth of field option as well using the thin lens model approximation. More
realistic camera will be added to the future.

### Textures and images
ggr fully supports image textures but only supports PPM files for loading them. ggr only supports PPM files as the only
option to save rendered images as well. This was done for the simplicity of the PPM file description and in the future
more file formats will be added. For texture filtering, ggr has not only the typical bilinear and trilinear filters but
also an anisotropic EWA filter that is used in most renderers.

### Speed and accelerators
ggr is written with many optimizations in mind but it currently has no accelerators. These will be added in the future.
To decrease the rendering speed ggr is also multithreaded.

## Compiling and running
ggr does not a have main function right now. In order to create run, you must write a cpp file that contains a main function
and include the you need yourself. You can see some examples in the examples folder. Although the current workflow is tedious,
the final goal is to add a scripting like like that the renderer will parse.
To compile a project, use the g++ compiler and add the **-Isource** flag to include the files you need. You should also
include the **-pthread** flag for the thread includes.
Example: g++ -Isource {source files} -O3 -pthread -o ggrtest tests/test.cpp

## Bugs and additions
If you find a bug or want an addition, open an issue and I will look into it. If you want to contribute, you can open
a pull request and I will look into it as well. I will write a more detailed documentation of the renderer and some
rules that I followed when writing it.

