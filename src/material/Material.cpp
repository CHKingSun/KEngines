#include "Material.h"
#include "../render/Shader.h"

namespace KEngines { namespace KMaterial {
	const std::string Material::U_AMBIENT{ "u_ambient" };
	const std::string Material::U_DIFFUSE{ "u_diffuse" };
	const std::string Material::U_SPECULAR{ "u_specular" };
	const std::string Material::U_SHININESS{ "u_shininess" };
	const std::string Material::U_OPACITY{ "u_opacity" };

	Material::Material(const vec3& ma /* = GREY */, const vec3& md /* = GREY */,
		const vec3& ms /* = GREY */, Kfloat msh /* = 1.f */, Kfloat mo /* = 1.f */) :
		ambient(ma), diffuse(md), specular(ms), shininess(msh), opacity(mo), textures() {}

	Material::Material(const std::vector<Texture *>& tex, const vec3& ma /* = GREY */,
		const vec3& md /* = GREY */, const vec3& ms /* = GREY */,
		Kfloat msh /* = 1.f */, Kfloat mo /* = 1.f */) : textures(tex),
		ambient(ma), diffuse(md), specular(ms), shininess(msh), opacity(mo) {
		Kuint unit = 2;
		for (const auto& it : textures) {
			it->setTextureUnit(unit++);
		}
	}

	Material::~Material() {
		for (auto& tex : textures) {
			delete tex;
			tex = nullptr;
		}
		textures.clear();
	}

	void Material::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();
		shader->bindUniform3f(U_AMBIENT.c_str(), ambient);
		shader->bindUniform3f(U_DIFFUSE.c_str(), diffuse);
		shader->bindUniform3f(U_SPECULAR.c_str(), specular);
		shader->bindUniform1f(U_SHININESS.c_str(), shininess);
		shader->bindUniform1f(U_OPACITY.c_str(), opacity);

		for (const auto& tex : textures) {
			tex->bindUniform(shader);
		}
	}

	void Material::unActiveTextures(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("Shader is nullptr, unable to unbind texture!");
			return;
		}

		for (const auto& tex : textures) {
			tex->unActive(shader);
		}
	}

	Kboolean Material::addTexture(Texture* tex) {
		if (tex == nullptr) {
			Log::error("The texture to add is nullptr!");
			return false;
		}

		textures.emplace_back(tex);
		tex->setTextureUnit(textures.size() + 1);
		return true;
	}

	Kboolean Material::deleteTexture(Kuint index) {
		if (index >= textures.size()) {
			Log::error("The texture you want to delete does not exist!");
			return false;
		}

		auto it = textures.begin();
		std::advance(it, index);
		delete (*it); (*it) = nullptr;
		textures.erase(it);

		Kuint unit = 2;
		for (const auto& it : textures) {
			it->setTextureUnit(unit++);
		}

		return true;
	}
} }