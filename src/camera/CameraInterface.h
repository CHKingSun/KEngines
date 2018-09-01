//
// Create by KingSun on 2018/08/30
//

#ifndef KENGINES_CAMERA_INTERFACE_H
#define KENGINES_CAMERA_INTERFACE_H

#include "../KHeader.h"
#include "../Log.h"
#include "../math/matrix/quaternion.h"
#include "../math/transform.h"

namespace KEngines { namespace KRenderer { class Shader; } }

enum DirectionType {
	RIGHT = 1, LEFT, BACK, FORWARD, UP, DOWN
};

namespace KEngines { namespace KCamera {
	using namespace KVector;
	using namespace KMatrix;

	class CameraInterface {
	protected:
		static const std::string U_PROJ;
		static const std::string U_VIEW_POS;

		vec3 position;
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

		virtual void updateMatrix() = 0;

	public:
		explicit CameraInterface(const vec3& pos = vec3());
		CameraInterface(const vec3& eye, const vec3& center, const vec3& up);
		CameraInterface(Kfloat fovy, Kfloat aspect, Kfloat zNear, Kfloat zFar, const vec3 &pos);
		virtual ~CameraInterface() = default;

		virtual void bindUniform(const KRenderer::Shader* shader)const = 0;

		void setView(const vec3& eye, const vec3& center, const vec3& up);

		void setPostion(const vec3& pos) {
			position = pos;
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

		const mat4& getProjectionMatrix()const { return projection; }

		virtual void rotateView(const quaternion& rot) {
			view_rotation *= rot;
			update();
		}
		virtual vec3 getPosition()const = 0;
		virtual vec3 getDirection(DirectionType type = FORWARD)const = 0;
		virtual mat3 getDirectionMatrix()const = 0;
	};
} }

#endif // !KENGINES_CAMERA_INTERFACE_H

