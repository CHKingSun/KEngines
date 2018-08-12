#include "BasicLight.h"
#include "../render/Shader.h"

namespace KEngines { namespace KLight {
	void BasicLight::active(const KRenderer::Shader* shader, Kboolean enable /* = true */)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		if (enable) {
			shader->bindUniform1i(u_enable.c_str(), true);
		}
		else {
			shader->bindUniform1i(u_enable.c_str(), false);
		}
	}

	void BasicLight::setBindId(Kuint bind_id) {
		static const Kuint index = 10; //The number index in the string.
		std::string id = std::to_string(bind_id);
		if (this->bind_id < 10) {
			u_enable.replace(index, 1, id);
			u_intensity.replace(index, 1, id);
			u_ambient.replace(index, 1, id);
		}
		else {
			u_enable.replace(index, 2, id);
			u_intensity.replace(index, 2, id);
			u_ambient.replace(index, 2, id);
		}
		
		this->bind_id = bind_id;
	}

	void BasicLight::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();
		shader->bindUniform1i(u_enable.c_str(), true);
		shader->bindUniform1f(u_intensity.c_str(), intensity);
		shader->bindUniform3f(u_ambient.c_str(), ambient);
	}
} }