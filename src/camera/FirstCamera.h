//
// Created by KingSun on 2018/08/30
//

#ifndef KENGINES_FIRST_CAMERA_H
#define KENGINES_FIRST_CAMERA_H

#include "CameraInterface.h"

namespace KEngines { namespace KCamera {

	class FirstCamera : public CameraInterface {
	protected:
		Kfloat restrict_angle;
		const static vec3 restrict_axis;

	public:
		void updateMatrix()override;

	public:
		explicit FirstCamera(const vec3& pos = vec3(), Kfloat angle = 30.f) :
			CameraInterface(pos), restrict_angle(angle) { updateMatrix(); }
		FirstCamera(const vec3& eye, const vec3& center, const vec3& up, Kfloat angle = 30.f) :
			CameraInterface(eye, center, up), restrict_angle(angle) { updateMatrix(); }
		FirstCamera(Kfloat fovy, Kfloat aspect, Kfloat zNear, Kfloat zFar,
			const vec3 &pos = vec3(), Kfloat angle = 15.f) :
			CameraInterface(fovy, aspect, zNear, zFar, pos), restrict_angle(angle) { updateMatrix(); }
		~FirstCamera() = default;

		void setRestrictAngle(Kfloat angle) { restrict_angle = angle; }

		void bindUniform(const KRenderer::Shader* shader)const override;

		void rotateView(const quaternion& rot)override;

		void rotateToDirection(const vec3& direction);

		vec3 getPosition()const override { return position; }
		vec3 getDirection(DirectionType type /* = FORWARD */)const override;
		mat3 getDirectionMatrix()const override { return (-view_rotation).toMat3(); }
	};
} }

#endif // !KENGINES_FIRST_CAMERA_H
