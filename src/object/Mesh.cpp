#include "Mesh.h"
#include "../buffer/VertexArray.h"
#include "../material/Material.h"

namespace KEngines { namespace KObject {
	Mesh::Mesh(const std::vector<Vertex>* vs, const std::vector<Ksize>* is, KMaterial::Material* mat):
		Object3D("Mesh"), vertices(vs), indices(is), count(is->size()) {
		setMaterial(mat);

		initBuffer();
	}

	Mesh::~Mesh() {
		delete vertices;
		delete indices;
	}

	void Mesh::initBuffer() {
		if (vao) delete vao;
		vao = new KBuffer::VertexArray();

		if (vbo) delete vbo;
		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX,
							sizeof(Vertex) * vertices->size(), vertices->data());

		vao->allocate(vbo, A_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, vertex));
		vao->allocate(vbo, A_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal));
		vao->allocate(vbo, A_TEX_COORD, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, tex_coord));

		if (ibo) delete ibo;
		ibo = new KBuffer::VertexBuffer(KBuffer::INDEX,
							sizeof(Ksize) * indices->size(), indices->data());

		delete vertices; vertices = nullptr;
		delete indices; indices = nullptr;
	}

	void Mesh::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		if (material != nullptr) material->bindUniform(shader);
	}

	void Mesh::render(const KRenderer::Shader* shader /* = nullptr */)const {
		bind();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		unBind();
		if (material != nullptr) material->unActiveTextures(shader);
	}
} }