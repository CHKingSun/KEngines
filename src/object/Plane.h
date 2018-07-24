//
// Created by KingSun on 2018/07/23
//

#ifndef KENGINES_PLANE_H
#define KENGINES_PLANE_H

#include <vector>
#include "Object3D.h"

namespace KEngines { namespace KObject {
	class Plane : public Object3D {
	private:
		std::vector<vec2>* vertices;
		std::vector<vec2>* tex_coords;

		std::vector<Ksize>* indices;

		Kuint count;

		void generate(Kfloat width, Kfloat height, Ksize xslices, Ksize yslices);

		void initABuffer();

	public:
		Plane(Kfloat width = 1.0f, Kfloat height = 1.0f,
			Ksize xslices = 1, Ksize yslices = 1);

		~Plane();

		void render(const KRenderer::Shader* shader = nullptr)const override;
	};
} }

#endif // KENGINES_PLANE_H

