//
// Created by KingSun on 2018/07/29
//

#ifndef KENGINES_CAMERA_H
#define KENGINES_CAMERA_H

#include "CameraInterface.h"

namespace KEngines { namespace KCamera {
	class Camera : public CameraInterface {
	protected:
		quaternion rotation;

	public:
		void updateMatrix() override;

	public:
		explicit Camera(const vec3& pos = vec3()) : CameraInterface(pos), rotation() { updateMatrix(); }
		Camera(const vec3& eye, const vec3& center, const vec3& up) :
			CameraInterface(eye, center, up), rotation() { updateMatrix(); }
		Camera(Kfloat fovy, Kfloat aspect, Kfloat zNear, Kfloat zFar, const vec3 &pos = vec3()) :
			CameraInterface(fovy, aspect, zNear, zFar, pos), rotation() { updateMatrix(); }
		~Camera() = default;

		void bindUniform(const KRenderer::Shader* shader)const override;

		void setViewRotation(const quaternion& rot) {
			view_rotation = rot;
			update();
		}
		void setCameraRotation(const quaternion& rot) {
			rotation = rot;
			update();
		}

		void rotateView(const quaternion& rot) {
			view_rotation = view_rotation * rot;
			update();
		}
		void rotateCamera(const quaternion& rot) {
			rotation = rotation * rot;
			update();
		}

		vec3 getPosition()const override {
			return rotation * position;
		}
		vec3 getDirection(DirectionType type)const override;
		mat3 getDirectionMatrix()const override {
			return (-rotation * view_rotation).toMat3();
		}
	};
} }

#endif // !KENGINES_CAMERA_H
