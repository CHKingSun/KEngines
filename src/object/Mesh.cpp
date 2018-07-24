#include "Mesh.h"
#include "../buffer/VertexArray.h"

namespace KEngines { namespace KObject {
	Mesh::Mesh(): Object3D("Mesh"),
		vertices(nullptr), indices(nullptr) {}

	Mesh::~Mesh() {
		delete vertices;
		delete indices;
	}

	void Mesh::initBuffer() {
		vao = new KBuffer::VertexArray();

		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX,
							sizeof(Vertex) * vertices->size(), vertices->data());

		vao->allocate(vbo, A_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, vertex));
		vao->allocate(vbo, A_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal));
		vao->allocate(vbo, A_TEX_COORD, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, tex_coord));

		ibo = new KBuffer::VertexBuffer(KBuffer::INDEX,
							sizeof(Ksize) * indices->size(), vertices->data());

		delete vertices; vertices = nullptr;
		delete indices; indices = nullptr;
	}

	void Mesh::render()const {
		bind();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		unBind();
	}
} }