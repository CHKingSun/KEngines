#include "Texture.h"
#include "../render/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace KEngines { namespace KMaterial {
	const std::string Texture::TEX_HEAD{ "u_textures[" }; // = "u_textures[";
	const std::string Texture::TEX_TEXTURE{ "].tex" }; // = "].tex";
	const std::string Texture::TEX_TYPE{ "].type" }; // = "].type";
	//const std::string Texture::TEX_ENABLE{ "].enable" }; // = "].enable";

	Kint Texture::max_tex_num = 0;
	std::unordered_map<std::string, TextureCount> Texture::tex_msg{};

	Texture::Texture(const std::string& path, TextureType type /* = AMBIENT */) :
		path(path), type(type), active_id(0), tex_id(0) {
		if (max_tex_num == 0) {
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_tex_num);
			Log::info("Max texture image uints: ", max_tex_num);
		}
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
			GLenum format;
			switch (component) {
			case 1:
				format = GL_R3_G3_B2;
				break;
			case 2:
				format = GL_RGBA4;
				break;
			case 3:
				format = GL_RGB8;
				break;
			case 4:
				format = GL_RGBA8;
				break;
			default:
				format = GL_RGB8;
			}

			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

	void Texture::bindUniform(const KRenderer::Shader* shader, Kuint active_id)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		if (active_id >= max_tex_num) {
			Log::error("Texture image uints is out of range!");
		}
		else if (this->active_id != active_id) {
			this->active_id = active_id;
		}

		glActiveTexture(GL_TEXTURE0 + this->active_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		const std::string index = std::to_string(this->active_id);
		shader->bindUniform1i((TEX_HEAD + index + TEX_TEXTURE).c_str(), this->active_id);
		shader->bindUniform1i((TEX_HEAD + index + TEX_TYPE).c_str(), type);
		//shader->bindUniform1i((TEX_HEAD + index + TEX_ENABLE).c_str(), 1);
	}

	void Texture::unActive(const KRenderer::Shader* shader) {
		//glActiveTexture(GL_TEXTURE0 + active_id);
		//glBindTexture(GL_TEXTURE_2D, 0);

		shader->bindUniform1i((TEX_HEAD + std::to_string(this->active_id) + TEX_TYPE).c_str(), NONE);
	}
} }