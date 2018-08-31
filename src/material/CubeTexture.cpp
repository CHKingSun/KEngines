#include "CubeTexture.h"
#include "../render/Shader.h"
#include "../camera/CameraInterface.h"
#include "../buffer/VertexArray.h"

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace KEngines { namespace KMaterial {
	const std::string CubeTexture::U_CUBE_TEX{ "u_cube_tex" };

	CubeTexture::CubeTexture(const std::vector<std::string>& paths, Kuint tex_unit /* = 0 */) :
		paths(6, IMAGE_PATH + "default.png"), tex_unit(tex_unit),
		map_shader(nullptr), vao(nullptr), vbo(nullptr), ibo(nullptr) {
		if (paths.size() < 6) {
			Log::info("Textures are less than 6 in cube texture, use default texture instead.");
			for (int i = 0; i < paths.size(); ++i) {
				this->paths[i] = paths[i];
			}
		}
		else {
			for (int i = 0; i < 6; ++i) {
				this->paths[i] = paths[i];
			}
		}

		initGL();
		tex_id = getCubeTextrueId(paths);
	}

	void CubeTexture::initGL() {
		map_shader = new KRenderer::Shader(SHADER_PATH + "cube_map.vert", SHADER_PATH + "cube_map.frag");

		vao = new KBuffer::VertexArray();

		const KVector::vec3 vertices[8] = {
			{ -1.f, -1.f,  1.f }, {  1.f, -1.f,  1.f },
			{  1.f,  1.f,  1.f }, { -1.f,  1.f,  1.f },
			{ -1.f, -1.f, -1.f }, {  1.f, -1.f, -1.f },
			{  1.f,  1.f, -1.f }, { -1.f,  1.f, -1.f }
		};
		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vertices), vertices);
		vao->allocate(vbo, 0, 3);

		const Kubyte indices[36] = {
			0, 1, 2, 2, 3, 0,
			1, 5, 6, 6, 2, 1,
			5, 4, 7, 7, 6, 5,
			4, 0, 3, 3, 7, 4,
			3, 2, 6, 6, 7, 3,
			4, 5, 1, 1, 0, 4
		};
		ibo = new KBuffer::VertexBuffer(KBuffer::INDEX, sizeof(indices), indices);

		bindUniform(map_shader);
	}

	CubeTexture::~CubeTexture() {
		if (glIsTexture(tex_id)) glDeleteTextures(1, &tex_id);

		delete vbo;
		delete ibo;
		delete vao;
		delete map_shader;
	}

	Kuint CubeTexture::getCubeTextrueId(const std::vector<std::string>& paths) {
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		int index = 0;
		for (const auto& path : paths) {
			int width, height, component;
			GLubyte *data = stbi_load(path.data(), &width, &height, &component, 0);
			if (data != nullptr) {
				GLenum format, type;
				switch (component) {
				case 1:
					format = GL_RGB;
					type = GL_UNSIGNED_BYTE_3_3_2;
					break;
				case 2:
					format = GL_RGBA;
					type = GL_UNSIGNED_SHORT_4_4_4_4;
					break;
				case 3:
					format = GL_RGB;
					type = GL_UNSIGNED_BYTE;
					break;
				case 4:
					format = GL_RGBA;
					type = GL_UNSIGNED_BYTE;
					break;
				default:
					format = GL_RGB;
					type = GL_UNSIGNED_BYTE;
				}

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB8, width, height, 0, format, type, data);
			}
			else {
				Log::error("Load texture file: ", path, " failed!");
			}

			++index;
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//Note: do not use _MIPMAP_ macros if you do not want to generate mipmap.
		//The default GL_TEXTURE_MIN_FILTER parameter is GL_NEAREST_MIPMAP_LINEAR,
		//so remember to change it.
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//The default GL_TEXTURE_MAG_FILTER paraameter is GL_LINEAR.

		return id;
	}

	void CubeTexture::bindMatrix(const KCamera::CameraInterface* camera)const {
		if (camera == nullptr) {
			Log::error("Camera is nullptr!");
			return;
		}

		map_shader->bind();
		map_shader->bindUniformMat3f("u_rotation", camera->getDirectionMatrix());
		map_shader->bindUniformMat4f("u_projection", camera->getProjectionMatrix());
	}

	void CubeTexture::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();
		glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
		shader->bindUniform1i(U_CUBE_TEX.c_str(), tex_unit);
	}

	void CubeTexture::unActive(const KRenderer::Shader* shader /* = nullptr */)const {
		glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void CubeTexture::render()const {
		map_shader->bind();
		vao->bind();
		ibo->bind();

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr);

		vao->unBind();
		ibo->unBind();
	}
} }