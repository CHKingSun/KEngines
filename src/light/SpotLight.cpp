#include "SpotLight.h"
#include "../render/Shader.h"

namespace KEngines {
	namespace KLight {
		void SpotLight::setBindId(Kuint bind_id) {
			static const Kuint index = 10; //The number index in the string.
			std::string id = std::to_string(bind_id);
			if (this->bind_id < 10) {
				u_position.replace(index, 1, id);
				u_direction.replace(index, 1, id);
				u_diffuse.replace(index, 1, id);
				u_specular.replace(index, 1, id);
				u_kq.replace(index, 1, id);
				u_kl.replace(index, 1, id);
				u_kc.replace(index, 1, id);
				u_inner_cutoff.replace(index, 1, id);
				u_outer_cutoff.replace(index, 1, id);
			}
			else {
				u_position.replace(index, 2, id);
				u_direction.replace(index, 2, id);
				u_diffuse.replace(index, 2, id);
				u_specular.replace(index, 2, id);
				u_kq.replace(index, 2, id);
				u_kl.replace(index, 2, id);
				u_kc.replace(index, 2, id);
				u_inner_cutoff.replace(index, 2, id);
				u_outer_cutoff.replace(index, 2, id);
			}

			BasicLight::setBindId(bind_id);
		}

		void SpotLight::bindUniform(const KRenderer::Shader* shader)const {
			if (shader == nullptr) {
				Log::error("The shader to bind unifrom is null!");
				return;
			}

			BasicLight::bindUniform(shader);
			shader->bindUniform3f(u_position.c_str(), position);
			shader->bindUniform3f(u_direction.c_str(), direction);
			shader->bindUniform3f(u_diffuse.c_str(), diffuse);
			shader->bindUniform3f(u_specular.c_str(), specular);
			shader->bindUniform1f(u_kq.c_str(), kq);
			shader->bindUniform1f(u_kl.c_str(), kl);
			shader->bindUniform1f(u_kc.c_str(), kc);
			shader->bindUniform1f(u_inner_cutoff.c_str(), inner_cutoff);
			shader->bindUniform1f(u_outer_cutoff.c_str(), outer_cutoff);
		}

		void SpotLight::bindPosition(const KRenderer::Shader* shader)const {
			if (shader == nullptr) {
				Log::error("The shader to bind unifrom is null!");
				return;
			}

			shader->bind();
			shader->bindUniform3f(u_position.c_str(), position);
		}

		void SpotLight::bindDirection(const KRenderer::Shader* shader)const {
			if (shader == nullptr) {
				Log::error("The shader to bind unifrom is null!");
				return;
			}

			shader->bind();
			shader->bindUniform3f(u_direction.c_str(), direction);
		}
	}
}