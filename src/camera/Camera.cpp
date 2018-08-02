#include "Camera.h"
#include "../render/Shader.h"

namespace KEngines { namespace KCamera {
	const std::string Camera::U_PROJ{ "u_proj_matrix" };

	Camera::Camera(const vec3& pos /* = vec3() */) :
		position(pos), rotation(), view_rotation(), auto_update(true) {
		setOrthoProjection(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
	}

	Camera::Camera(const vec3& eye, const vec3& center, const vec3& up) :
		rotation(), auto_update(true) {
		setOrthoProjection(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
		setView(eye, center, up);
	}

	Camera::Camera(Kfloat fovy, Kfloat aspect, Kfloat zNear,
		Kfloat zFar, const vec3 &pos /* = vec3() */) :
		position(pos), rotation(), view_rotation(), auto_update(true) {
		setPerspectiveProjection(fovy, aspect, zNear, zFar);
	}

	void Camera::updateMatrix() {
		if (!auto_update) return;
		proj_view_matrix = projection;
		mat3 tmp = (-view_rotation).toMat3();
		proj_view_matrix *= (mat4(tmp, tmp * -position) *= (-rotation).toMat4());
	}

	void Camera::bindUnifrom(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}
		shader->bind();
		shader->bindUniformMat4f(U_PROJ.c_str(), proj_view_matrix);
	}

	void Camera::setView(const vec3& eye, const vec3& center, const vec3& up) {
		//u-v-n is left-hand coordinate
		const vec3 n((center - eye).normalize());
		const vec3 u(vec3::cross(n, up).normalize());
		const vec3 v(vec3::cross(u, n).normalize());

		position = eye;
		view_rotation.fromMatrix(mat3(u, v, -n));
		view_rotation.inverse();

		update();
	}
} }