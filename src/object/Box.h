//
// Created by KingSun on 2018/08/15
//

#ifndef KENGINES_BOX_H
#define KENGINES_BOX_H

#include <vector>
#include "Object3D.h"

namespace KEngines { namespace KObject {
	class Box : public Object3D {
	private:
		std::vector<vec3>* vertices;
		std::vector<vec3>* normals;
		std::vector<vec2>* tex_coords;

		std::vector<Ksize>* indices;

		Kuint count;

		void generatePlane(Kfloat width, Kfloat height, Ksize xslices, Ksize yslices, Kfloat depth, const Kuint depth_axis);

		void initBuffer();

	public:
		Box(Kfloat width = 1.0f, Kfloat height = 1.0f, Kfloat depth = 1.f,
			Ksize xslices = 1, Ksize yslices = 1, Ksize zslices = 1);

		~Box();

		void render(const KRenderer::Shader* shader = nullptr)const override;
	};
} }

#endif // !KENGINES_BOX_H

