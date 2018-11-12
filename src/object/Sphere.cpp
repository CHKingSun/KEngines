#include "Sphere.h"
#include "../buffer/VertexArray.h"
#include "../material/Material.h"

namespace KEngines { namespace KObject {
	Sphere::Sphere(Kfloat radius /* = 1.f */, Ksize aslices /* = 20 */,
		Ksize rslices /* = 20 */) : Object3D("Sphere"), radius(radius),
		vertices(nullptr), tex_coords(nullptr), indices(nullptr) {
		material = new KMaterial::Material(BLACK, GREY, GREY, 20);
		material->addTexture(new KMaterial::Texture(IMAGE_PATH + "earth.jpg", KMaterial::DIFFUSE));
		material->addTexture(new KMaterial::Texture(IMAGE_PATH + "earth.jpg", KMaterial::SPECULAR));

		generate(aslices, rslices);
		initBuffers();
	}

	Sphere::~Sphere() {
		delete vertices;
		delete tex_coords;
		delete indices;
	}

	void Sphere::generate(Ksize aslices, Ksize rslices) {
		count = ((rslices - 1) * 4 + 1) * aslices + 1;
		Ksize v_count = (aslices + 1) * (rslices - 1) * 2 + 2;
		vertices = new std::vector<vec3>();
		vertices->reserve(v_count);
		tex_coords = new std::vector<vec2>();
		tex_coords->reserve(v_count);
		indices = new std::vector<Kuint>();
		indices->reserve(count);

		Kfloat per_xangle = PI * 2.f / aslices;
		Kfloat per_yangle = PI / (rslices * 2.f);
		Kfloat pertx = 1.f / aslices;
		Kfloat perty = 0.5f / rslices;
		Kfloat yangle = PI - per_yangle; //form bottom to top(except two poles).
		Kfloat ty = 1.f - perty;
		for (int i = 0; i < ((rslices - 1) << 1); ++i, yangle -= per_yangle, ty -= perty) {
			Kfloat y = radius * cos(yangle);
			Kfloat r = radius * sin(yangle);
			Kfloat xangle = 0; //draw a circle
			Kfloat tx = 0;
			for (int j = 0; j <= aslices; ++j, xangle += per_xangle, tx += pertx) {
				vertices->emplace_back(cos(xangle) * r, y, sin(xangle) * r);
				tex_coords->emplace_back(1.f - tx, ty);
			}
		}
		vertices->emplace_back(0.f, -radius, 0.f); //bottom pole [v_count - 2]
		tex_coords->emplace_back(1.f, 1.f);
		vertices->emplace_back(0.f, radius, 0.f); //top pole [v_count - 1]
		tex_coords->emplace_back(0.f, 0.f);

		int rt = aslices + 1;
		indices->emplace_back(v_count - 2);
		Ksize index = 0;
		for (Ksize at = 0; at < aslices; ++at, ++index) {
			for (int i = 0; i < ((rslices - 1) << 1); ++i, index += rt) {
				indices->emplace_back(index);
				indices->emplace_back(index + 1);
			}
			index -= rt;
			if (rt > 0) indices->emplace_back(v_count - 1);
			else indices->emplace_back(v_count - 2);
			rt = -rt;
		}
	}

	void Sphere::initBuffers() {
		vao = new KBuffer::VertexArray();

		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, vertices->size() * sizeof(vec3), vertices->data());
		vao->allocate(vbo, A_POSITION, 3);

		tbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, tex_coords->size() * sizeof(vec2), tex_coords->data());
		vao->allocate(tbo, A_TEX_COORD, 2);

		vao->allocate(vbo, A_NORMAL, 3);

		ibo = new KBuffer::VertexBuffer(KBuffer::INDEX, indices->size() * sizeof(Kuint), indices->data());

		delete vertices; vertices = nullptr;
		delete tex_coords; tex_coords = nullptr;
		delete indices; indices = nullptr;
	}

	void Sphere::render(const KRenderer::Shader* shader /* = nullptr */)const {
		bind();

		glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, nullptr);

		unBind();
		if (material != nullptr) material->unActiveTextures(shader);
	}
} }