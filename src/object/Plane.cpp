#include "Plane.h"
#include "../buffer/VertexArray.h"
#include "../material/Material.h"

namespace KEngines { namespace KObject {
	Plane::Plane(Kfloat width /* = 1.0f */, Kfloat height /* = 1.0f */,
		Ksize xslices /* = 1 */, Ksize yslices /* = 1 */): Object3D("Plane"),
		vertices(nullptr), tex_coords(nullptr), indices(nullptr),
		count(xslices * yslices * 6) {
		vertices = new std::vector<vec2>();
		vertices->reserve(count * 2 / 3);
		tex_coords = new std::vector<vec2>();
		tex_coords->reserve(count * 2 / 3);
		indices = new std::vector<Kuint>();
		indices->reserve(count);

		material = new KMaterial::Material();

		generate(width, height, xslices, yslices);
		initBuffer();
	}

	Plane::~Plane() {
		delete vertices;
		delete tex_coords;
		delete indices;
	}

	void Plane::generate(Kfloat width, Kfloat height, Ksize xslices, Ksize yslices) {
		Kfloat px = -width / 2.0f;
		Kfloat py = -height / 2.0f;
		Kfloat perx = width / xslices;
		Kfloat pery = height / yslices;
		for (unsigned int i = 0; i < xslices; ++i) {
			for (unsigned int j = 0; j < yslices; ++j) {
				Kfloat x = px + perx * i;
				Kfloat y = py + pery * j;

				vertices->emplace_back(x, y);
				vertices->emplace_back(x + perx, y);
				vertices->emplace_back(x + perx, y + pery);
				vertices->emplace_back(x, y + pery);

				tex_coords->emplace_back(0.0f, 0.0f);
				tex_coords->emplace_back(1.0f, 0.0f);
				tex_coords->emplace_back(1.0f, 1.0f);
				tex_coords->emplace_back(0.0f, 1.0f);

				Kuint t = vertices->size() - 4;
				indices->emplace_back(t);
				indices->emplace_back(t + 1);
				indices->emplace_back(t + 2);
				indices->emplace_back(t + 2);
				indices->emplace_back(t + 3);
				indices->emplace_back(t);
			}
		}
	}

	void Plane::initBuffer() {
		vao = new KBuffer::VertexArray();

		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec2) * vertices->size(), vertices->data());
		vao->allocate(vbo, A_POSITION, 2);

		tbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec2) * tex_coords->size(), tex_coords->data());
		vao->allocate(tbo, A_TEX_COORD, 2);

		vao->setVertexAttrib3f(A_NORMAL, vec3(0.0f, 0.0f, 1.0f));

		ibo = new KBuffer::VertexBuffer(KBuffer::INDEX, sizeof(Ksize) * indices->size(), indices->data());

		delete vertices; vertices = nullptr;
		delete tex_coords; tex_coords = nullptr;
		delete indices; indices = nullptr;
	}

	void Plane::render(const KRenderer::Shader* shader /* = nullptr */)const {
		bind();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		unBind();
		if (material != nullptr) material->unActiveTextures(shader);
	}
} }