#include "Texture.h"
#include "../render/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace KEngines { namespace KMaterial {

	std::unordered_map<std::string, TextureCount> Texture::tex_msg{};

	Texture::Texture(const std::string& path, TextureType type /* = AMBIENT */,
		Kuint tex_unit /* = 0 */) : path(path), type(type), tex_id(0), tex_unit(tex_unit),
		u_tex("u_textures[i].tex"), u_type("u_textures[i].type") {
		setTextureUnit(tex_unit);

		auto fit = tex_msg.find(path);
		if (fit != tex_msg.end()) {
			fit->second.count += 1;
			tex_id = fit->second.tex_id;
			return;
		}
		tex_id = getTextureId(path);
		if (tex_id != 0) tex_msg.emplace(path, tex_id);
	}

	Texture::~Texture() {
		auto fit = tex_msg.find(path);
		if (fit != tex_msg.end()) {
			fit->second.count -= 1;
			if (fit->second.count == 0) {
				if (glIsTexture(tex_id)) glDeleteTextures(1, &tex_id);
				tex_msg.erase(fit);
			}
		}
	}

	Kuint Texture::getTextureId(const std::string& path) {
		GLuint id;
		glGenTextures(1, &id);

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

			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, format, type, data);
			//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);
			//OpenGL 4.5 glGenerateTextureMipmap(texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			//Note: do not use _MIPMAP_ macros if you do not want to generate mipmap.
			//The default GL_TEXTURE_MIN_FILTER parameter is GL_NEAREST_MIPMAP_LINEAR,
			//so remember to change it.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//The default GL_TEXTURE_MAG_FILTER paraameter is GL_LINEAR.

			stbi_image_free(data);
		}
		else {
			Log::error("Load texture file: ", path, " failed!");
			glDeleteTextures(1, &id);
			return 0;
		}
		return id;
	}

	void Texture::setTextureUnit(Kuint unit) {
		Kint max_tex_num = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_tex_num);
		if (max_tex_num <= unit) {
			Log::error("Texture unit cannot be more than ", max_tex_num);
			return;
		}

		if (tex_unit < 10) {
			u_tex.replace(11, 1, std::to_string(unit));
			u_type.replace(11, 1, std::to_string(unit));
		}
		else {
			u_tex.replace(11, 2, std::to_string(unit));
			u_type.replace(11, 2, std::to_string(unit));
		}

		tex_unit = unit;
	}

	void Texture::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		shader->bindUniform1i(u_tex.c_str(), tex_unit);
		shader->bindUniform1i(u_type.c_str(), type);
	}

	void Texture::unActive(const KRenderer::Shader* shader)const {
		//glActiveTexture(GL_TEXTURE0 + tex_unit);
		//glBindTexture(GL_TEXTURE_2D, 0);

		shader->bindUniform1i(u_type.c_str(), NONE);
	}
} }