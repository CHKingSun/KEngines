#include "DirectionLight.h"
#include "../render/Shader.h"
#include "../math/transform.h"

namespace KEngines { namespace KLight {
	void DirectionLight::setBindId(Kuint bind_id) {
		static const Kuint index = 10; //The number index in the string.
		std::string id = std::to_string(bind_id);
		if (this->bind_id < 10) {
			u_direction.replace(index, 1, id);
			u_diffuse.replace(index, 1, id);
			u_specular.replace(index, 1, id);
		}
		else {
			u_direction.replace(index, 2, id);
			u_diffuse.replace(index, 2, id);
			u_specular.replace(index, 2, id);
		}

		BasicLight::setBindId(bind_id);
	}

	void DirectionLight::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		BasicLight::bindUniform(shader);
		shader->bindUniform3f(u_direction.c_str(), direction);
		shader->bindUniform3f(u_diffuse.c_str(), diffuse);
		shader->bindUniform3f(u_specular.c_str(), specular);
	}

	void DirectionLight::bindDirection(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();
		shader->bindUniform3f(u_direction.c_str(), direction);
	}
} }