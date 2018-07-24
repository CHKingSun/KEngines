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
		std::vector<Vertex>* vertices;
		std::vector<Ksize>* indices;
		Ksize count;

	private:
		Mesh();

		void setVertices(std::vector<Vertex>* vs) { vertices = vs; }
		void setIndices(std::vector<Ksize>* is) {
			indices = is;
			count = indices->size();
		}

		void initBuffer();

	public:
		~Mesh();

		void render()const override;
	};
} }

#endif // !KENGINES_MESH_H
