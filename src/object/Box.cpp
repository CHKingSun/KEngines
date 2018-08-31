#include "Box.h"
#include "../buffer/VertexArray.h"
#include "../material/Material.h"

namespace KEngines { namespace KObject {
	Box::Box(Kfloat width /* = 1.0f */, Kfloat height /* = 1.0f */, Kfloat depth /* = 1.f */,
		Ksize xslices /* = 1 */, Ksize yslices /* = 1 */, Ksize zslices /* = 1 */) :
	Object3D("Box"), vertices(nullptr), tex_coords(nullptr), indices(nullptr), normals(nullptr) {
		count = (xslices * yslices + yslices * zslices + zslices * xslices) * 12;
		vertices = new std::vector<vec3>();
		vertices->reserve(count * 2 / 3);
		tex_coords = new std::vector<vec2>();
		tex_coords->reserve(count * 2 / 3);
		normals = new std::vector<vec3>();
		normals->reserve(count * 2 / 3);
		indices = new std::vector<Kuint>();
		indices->reserve(count);

		material = new KMaterial::Material();

		//Note: 0 -> x, 1 -> y, 2 -> z;
		generatePlane(width, height, xslices, yslices, depth, 2);
		generatePlane(depth, height, zslices, yslices, width, 1);
		generatePlane(width, depth, xslices, zslices, height, 0);
		initBuffer();
	}

	Box::~Box() {
		delete vertices;
		delete tex_coords;
		delete normals;
		delete indices;
	}

	void Box::generatePlane(Kfloat width, Kfloat height, Ksize xslices,
		Ksize yslices, Kfloat depth, const Kuint depth_axis) {
		const Kfloat px = -width / 2.f;
		const Kfloat py = -height / 2.f;
		const Kfloat halfZ = depth / 2.f;
		const Kfloat perx = width / xslices;
		const Kfloat pery = height / yslices;
		//positive axis
		for (unsigned int i = 0; i < xslices; ++i) {
			for (unsigned int j = 0; j < yslices; ++j) {
				const Kfloat x = px + perx * i;
				const Kfloat y = py + pery * j;

				if (depth_axis == 0) {
					vertices->emplace_back(halfZ, x, y);
					vertices->emplace_back(halfZ, x + perx, y);
					vertices->emplace_back(halfZ, x + perx, y + pery);
					vertices->emplace_back(halfZ, x, y + pery);

					normals->emplace_back(1.f, 0.f, 0.f);
					normals->emplace_back(1.f, 0.f, 0.f);
					normals->emplace_back(1.f, 0.f, 0.f);
					normals->emplace_back(1.f, 0.f, 0.f);
				}
				else if (depth_axis == 1) {
					vertices->emplace_back(x, halfZ, y);
					vertices->emplace_back(x + perx, halfZ, y);
					vertices->emplace_back(x + perx, halfZ, y + pery);
					vertices->emplace_back(x, halfZ, y + pery);

					normals->emplace_back(0.f, 1.f, 0.f);
					normals->emplace_back(0.f, 1.f, 0.f);
					normals->emplace_back(0.f, 1.f, 0.f);
					normals->emplace_back(0.f, 1.f, 0.f);
				}
				else {
					vertices->emplace_back(x, y, halfZ);
					vertices->emplace_back(x + perx, y, halfZ);
					vertices->emplace_back(x + perx, y + pery, halfZ);
					vertices->emplace_back(x, y + pery, halfZ);

					normals->emplace_back(0.f, 0.f, 1.f);
					normals->emplace_back(0.f, 0.f, 1.f);
					normals->emplace_back(0.f, 0.f, 1.f);
					normals->emplace_back(0.f, 0.f, 1.f);
				}

				tex_coords->emplace_back(0.f, 0.f);
				tex_coords->emplace_back(1.f, 0.f);
				tex_coords->emplace_back(1.f, 1.f);
				tex_coords->emplace_back(0.f, 1.f);

				Kuint t = vertices->size() - 4;
				indices->emplace_back(t);
				indices->emplace_back(t + 1);
				indices->emplace_back(t + 2);
				indices->emplace_back(t + 2);
				indices->emplace_back(t + 3);
				indices->emplace_back(t);
			}
		}
		//negative axis
		for (unsigned int i = 0; i < xslices; ++i) {
			for (unsigned int j = 0; j < yslices; ++j) {
				const Kfloat x = px + perx * i;
				const Kfloat y = py + pery * j;

				if (depth_axis == 0) {
					vertices->emplace_back(-halfZ, x, y);
					vertices->emplace_back(-halfZ, x + perx, y);
					vertices->emplace_back(-halfZ, x + perx, y + pery);
					vertices->emplace_back(-halfZ, x, y + pery);

					normals->emplace_back(-1.f, 0.f, 0.f);
					normals->emplace_back(-1.f, 0.f, 0.f);
					normals->emplace_back(-1.f, 0.f, 0.f);
					normals->emplace_back(-1.f, 0.f, 0.f);
				}
				else if (depth_axis == 1) {
					vertices->emplace_back(x, -halfZ, y);
					vertices->emplace_back(x + perx, -halfZ, y);
					vertices->emplace_back(x + perx, -halfZ, y + pery);
					vertices->emplace_back(x, -halfZ, y + pery);

					normals->emplace_back(0.f, -1.f, 0.f);
					normals->emplace_back(0.f, -1.f, 0.f);
					normals->emplace_back(0.f, -1.f, 0.f);
					normals->emplace_back(0.f, -1.f, 0.f);
				}
				else {
					vertices->emplace_back(x, y, -halfZ);
					vertices->emplace_back(x + perx, y, -halfZ);
					vertices->emplace_back(x + perx, y + pery, -halfZ);
					vertices->emplace_back(x, y + pery, -halfZ);

					normals->emplace_back(0.f, 0.f, -1.f);
					normals->emplace_back(0.f, 0.f, -1.f);
					normals->emplace_back(0.f, 0.f, -1.f);
					normals->emplace_back(0.f, 0.f, -1.f);
				}

				tex_coords->emplace_back(1.f, 0.f);
				tex_coords->emplace_back(0.f, 0.f);
				tex_coords->emplace_back(0.f, 1.f);
				tex_coords->emplace_back(1.f, 1.f);

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

	void Box::initBuffer() {
		vao = new KBuffer::VertexArray();

		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec3) * vertices->size(), vertices->data());
		vao->allocate(vbo, A_POSITION, 3);

		tbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec2) * tex_coords->size(), tex_coords->data());
		vao->allocate(tbo, A_TEX_COORD, 2);

		nbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec3) * normals->size(), normals->data());
		vao->allocate(nbo, A_NORMAL, 3);

		ibo = new KBuffer::VertexBuffer(KBuffer::INDEX, sizeof(Ksize) * indices->size(), indices->data());

		delete vertices; vertices = nullptr;
		delete tex_coords; tex_coords = nullptr;
		delete normals; normals = nullptr;
		delete indices; indices = nullptr;
	}

	void Box::render(const KRenderer::Shader* shader /* = nullptr */)const {
		bind();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		unBind();
		if (material != nullptr) material->unActiveTextures(shader);
	}
} }