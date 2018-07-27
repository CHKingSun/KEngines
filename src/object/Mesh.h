//
// Created by KingSun on 2018/07/24
//

#ifndef KENGINES_MESH_H
#define KENGINES_MESH_H

#include <vector>
#include "Object3D.h"

namespace KEngines { namespace KObject {
	using namespace KVector;

	struct Vertex {
		Vertex(const vec3& v = vec3(),
			   const vec3& n = vec3(0.f, 1.f, 0.f),
			   const vec2& t = vec2()):
			vertex(v), normal(n), tex_coord(t) {}
		vec3 vertex;
		vec3 normal;
		vec2 tex_coord;
	};

	//A mesh is a part of model.
	class Mesh : public Object3D {
		friend class Model;

	private:
		const std::vector<Vertex>* vertices;
		const std::vector<Ksize>* indices;
		Ksize count;

	private:
		Mesh(const std::vector<Vertex>* vs, const std::vector<Ksize>* is);

		void initBuffer();

	public:
		~Mesh();

		void render(const KRenderer::Shader* shader = nullptr)const override;
	};
} }

#endif // !KENGINES_MESH_H
