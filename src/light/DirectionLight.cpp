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

	void DirectionLight::bindProjectionMatrix(const std::vector<vec3>& bounds,
		const KVector::vec3& pos, const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind matrix is null!");
			return;
		}

		KMatrix::mat4 proj = KFunction::lookAt(pos, pos - direction, UpVector);
		Kfloat x_min = FLT_MAX, y_min = FLT_MAX, z_min = FLT_MAX;
		Kfloat x_max = FLT_MIN, y_max = FLT_MIN, z_max = FLT_MIN;

		for (const auto& point : bounds) {
			vec4 p = proj * vec4(point, 1.f);

			x_min = min(x_min, p.x);
			y_min = min(y_min, p.y);
			z_min = min(z_min, p.z);
			x_max = max(x_max, p.x);
			y_max = max(y_max, p.y);
			z_max = max(z_max, p.z);
		}

		proj = KFunction::ortho(x_min, x_max, y_min, y_max, z_min, z_max) * proj;

		shader->bind();
		shader->bindUniformMat4f("u_light_matrix", proj);
	}
} }