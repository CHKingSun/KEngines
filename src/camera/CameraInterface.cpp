#include "CameraInterface.h"
#include "../render/Shader.h"

namespace KEngines { namespace KCamera {
	const std::string CameraInterface::U_PROJ{ "u_proj_matrix" };
	const std::string CameraInterface::U_VIEW_POS{ "u_view_pos" };

	CameraInterface::CameraInterface(const vec3& pos /* = vec3() */) :
		position(pos), view_rotation(), auto_update(true) {
		projection = KFunction::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
	}

	CameraInterface::CameraInterface(const vec3& eye,
		const vec3& center, const vec3& up): auto_update(true) {
		projection = KFunction::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

		//u-v-n is left-hand coordinate
		const vec3 n((center - eye).normalize());
		const vec3 u(vec3::cross(n, up).normalize());
		const vec3 v(vec3::cross(u, n).normalize());

		position = eye;
		view_rotation.fromMatrix(mat3(u, v, -n));
		view_rotation.inverse();
	}

	CameraInterface::CameraInterface(Kfloat fovy, Kfloat aspect,
		Kfloat zNear, Kfloat zFar, const vec3 &pos) :
		position(pos), view_rotation(), auto_update(true) {
		if (isZero(fovy) || isZero(aspect) ||
			isNan(fovy) || isNan(aspect)) {
			Log::error("fovy and aspect cannot be zero");
			return;
		}
		projection = KFunction::perspective(fovy, aspect, zNear, zFar);
	}

	void CameraInterface::setView(const vec3& eye, const vec3& center, const vec3& up) {
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