#include "FirstCamera.h"
#include "../math/vector/vec_default.h"
#include "../render/Shader.h"

namespace KEngines { namespace KCamera {
	const vec3 FirstCamera::restrict_axis = UpVector;

	void FirstCamera::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}
		shader->bind();
		shader->bindUniformMat4f(U_PROJ.c_str(), proj_view_matrix);
		shader->bindUniform3f(U_VIEW_POS.c_str(), position);
	}

	void FirstCamera::updateMatrix() {
		if (!auto_update) return;
		proj_view_matrix = projection;
		mat3 tmp = (view_rotation).toMat3(); //Y axis reversed view.
		//mat3 tmp = (-view_rotation).toMat3(); //Default view.
		proj_view_matrix *= (mat4(tmp, tmp * -position));
	}

	void FirstCamera::rotateView(const quaternion& rot) {
		quaternion rt = view_rotation;
		view_rotation *= rot;
		if (restrict_axis.getAngle(getDirection(FORWARD)) < restrict_angle
			|| restrict_axis.getAngle(getDirection(BACK)) < restrict_angle) {
			view_rotation = rt;
			return;
		}
		update();
	}

	void FirstCamera::rotateToDirection(const vec3& direction) {
		if (direction.y > 0.f && restrict_axis.getAngle(getDirection(FORWARD)) > restrict_angle) {
			view_rotation *= quaternion(ForwardVector.getAngle(vec3(0.f, direction.y, direction.z)),
				getDirection(RIGHT));
		}
		else if (direction.y < 0.f && restrict_axis.getAngle(getDirection(BACK)) > restrict_angle) {
			view_rotation *= quaternion(ForwardVector.getAngle(vec3(0.f, direction.y, direction.z)),
				getDirection(LEFT));
		}

 		if (!isZero(direction.x)) {
 			view_rotation *= quaternion(ForwardVector.getAngle(
 				vec3(direction.x, 0.f, direction.z)), vec3(0.f, direction.x, 0.f));
 		}
		update();

		//Log::info("Right direction : ", getDirection(RIGHT));
		//Log::info("Forward direction : ", getDirection(FORWARD));
		//Log::info("r * f : ", getDirection(FORWARD).dot(getDirection(RIGHT)));
	}

	vec3 FirstCamera::getDirection(DirectionType type)const {
		//Default forward direction is vec3(0.f, 0.f, -1.f).

		switch (type) {
		case RIGHT: return -view_rotation * RightVector;
		case LEFT: return -view_rotation * LeftVector;
		case BACK: return -view_rotation * BackVector;
		case FORWARD: return -view_rotation * ForwardVector;
		case UP: return -view_rotation * UpVector;
		case DOWN: return -view_rotation * DownVector;
		default:
			Log::error("Direction type you input is not support yet.");
		}

		return view_rotation * ForwardVector;
	}
} }