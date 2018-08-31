#include "Camera.h"
#include "../render/Shader.h"

namespace KEngines { namespace KCamera {
	void Camera::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}
		shader->bind();
		shader->bindUniformMat4f(U_PROJ.c_str(), proj_view_matrix);
		//shader->bindUniform3f(U_VIEW_POS.c_str(), position);
		shader->bindUniform3f(U_VIEW_POS.c_str(), (-rotation) * position);
	}

	void Camera::updateMatrix() {
		if (!auto_update) return;
		proj_view_matrix = projection;
		mat3 tmp = (-view_rotation).toMat3();
		proj_view_matrix *= (mat4(tmp, tmp * -position) *= (-rotation).toMat4());
	}

	vec3 Camera::getDirection(DirectionType type)const {
		//Default forward direction is vec3(0.f, 0.f, -1.f).
		quaternion rot = rotation * view_rotation;

		switch (type) {
		case RIGHT: return rot * vec3(1.f, 0.f, 0.f);
		case LEFT: return rot * vec3(-1.f, 0.f, 0.f);
		case BACK: return rot * vec3(0.f, 0.f, 1.f);
		case FORWARD: return rot * vec3(0.f, 0.f, -1.f);
		default:
			Log::error("Direction type you input is not support yet.");
		}

		return rot * vec3(0.f, 0.f, -1.f);
	}
} }