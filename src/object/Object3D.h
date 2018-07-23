//
// Created by KingSun on 2018/07/23
//

#ifndef KENGINES_OBJECT3D_H
#define KENGINES_OBJECT3D_H

#include <string>

#include "../math/matrix/quaternion.h"
#include "../Log.h"

namespace KEngines {
	namespace KBuffer { 
		class VertexArray;
		class VertexBuffer;
	}

	namespace KRenderer { class Shader; }
}

namespace KEngines { namespace KObject {
	using namespace KVector;
	using namespace KMatrix;

	class Object3D {
	protected:
		const static Kint A_POSITION; // = 0; a_position
		const static Kint A_TEX_COORD; // = 1; a_tex_coord
		const static Kint A_NORMAL; // = 2; a_normal

		const static char U_POSITION[]; //u_position
		const static char U_ROTATION[]; //u_rotation
		const static char U_SCALE[]; //u_scale

		KBuffer::VertexArray* vao;
		KBuffer::VertexBuffer* ibo;

		KBuffer::VertexBuffer* vbo;
		KBuffer::VertexBuffer* tbo;
		KBuffer::VertexBuffer* nbo;

		vec3 position;
		vec3 scale_size;
		quaternion rotation;

		const std::string type;

	protected:
		Object3D(const std::string& type);

	public:
		virtual ~Object3D();

		const std::string& getType()const { return type; }

		virtual void bind()const;

		virtual void unBind()const;

		virtual void bindUniform(const KRenderer::Shader* shader)const;

		//Remember to bind shader before you render it;
		virtual void render()const = 0;

		void setPosition(const vec3& v) { position = v; }

		void setRotation(const quaternion& q) { rotation = q; }

		void setScale(const vec3& v) { scale_size = v; }

		void translate(const vec3& v) { position += v; }

		void rotate(const quaternion& q) { rotation *= q; }

		void scale(const vec3& v) {
			if (v == vec3(0)) {
				Log::debug("Scale with zero may get valid result!");
			}

			scale_size *= v;
		}

		const vec3& getPostion()const { return position; }
	};
} }

#endif // !KENGINES_OBJECT3D_H

