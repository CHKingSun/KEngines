//
// Created by KingSun on 2018/09/02
//

#ifndef KENGINES_SPHERE_H
#define KENGINES_SPHERE_H

#include <vector>

#include "Object3D.h"

namespace KEngines { namespace KObject {
	class Sphere : public Object3D {
	private:
		Kfloat radius;
		Ksize count;

		std::vector<vec3> *vertices;
		std::vector<vec2> *tex_coords;
		std::vector<Kuint> *indices;

		void generate(Ksize aslices, Ksize rslices);

		void initBuffers();

	public:
		Sphere(Kfloat radius = 1.f, Ksize aslices = 20, Ksize rslices = 20);
		~Sphere();

		void render(const KRenderer::Shader* shader = nullptr)const override;

		const Kfloat& getRadius()const { return radius; }
	};
} }

#endif // !KENGINES_SPHERE_H
