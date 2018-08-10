//
// Created by KingSun on 2018/07/29
//

#ifndef KENGINES_CAMERA_H
#define KENGINES_CAMERA_H

#include "../KHeader.h"
#include "../Log.h"
#include "../math/matrix/quaternion.h"
#include "../math/transform.h"

namespace KEngines { namespace KRenderer {
	class Shader;
} }

namespace KEngines { namespace KCamera {
	using namespace KVector;
	using namespace KMatrix;

	class Camera {
	protected:
		const static std::string U_PROJ;
		const static std::string U_VIEW_POS;

		vec3 position;
		quaternion rotation;
		quaternion view_rotation;
		mat4 projection;
		mat4 proj_view_matrix;
		//proj_view_matrix = projection * (rotation * position * view_rotation).inverse()

		void update() {
			if (auto_update) updateMatrix();
		}

	public:
		//Whether proj_view_matrix will update once the values changed.
		//Default is true;
		//If you set false, you should call updateMatrix() yourself.
		Kboolean auto_update;

		virtual void updateMatrix();

	public:
		explicit Camera(const vec3& pos = vec3());
		Camera(const vec3& eye, const vec3& center, const vec3& up);
		Camera(Kfloat fovy, Kfloat aspect, Kfloat zNear, Kfloat zFar, const vec3 &pos = vec3());
		virtual ~Camera() = default;

		void bindUnifrom(const KRenderer::Shader* shader)const;

		void setView(const vec3& eye, const vec3& center, const vec3& up);

		void setPostion(const vec3& pos) {
			position = pos;
			update();
		}
		void setViewRotation(const quaternion& rot) {
			view_rotation = rot;
			update();
		}
		void setCameraRotation(const quaternion& rot) {
			rotation = rot;
			update();
		}

		void setPerspectiveProjection(Kfloat fovy, Kfloat aspect, Kfloat zNear, Kfloat zFar) {
			if (isZero(fovy) || isZero(aspect) ||
				isNan(fovy) || isNan(aspect)) {
				Log::error("fovy and aspect cannot be zero");
				return;
			}
			projection = KFunction::perspective(fovy, aspect, zNear, zFar);
			update();
		}
		void setOrthoProjection(Kfloat left, Kfloat right, Kfloat bottom,
								Kfloat top, Kfloat zNear, Kfloat zFar) {
			projection = KFunction::ortho(left, right, bottom, top, zNear, zFar);
			update();
		}
		void setFrustumProjection(Kfloat left, Kfloat right, Kfloat bottom,
									Kfloat top, Kfloat near, Kfloat far) {
			projection = KFunction::frustum(left, right, bottom, top, near, far);
			update();
		}

		void translate(const vec3& v) {
			position += v;
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

		vec3 getPosition()const {
			return rotation * position;
		}
	};
} }

#endif // !KENGINES_CAMERA_H
