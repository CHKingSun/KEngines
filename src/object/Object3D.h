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

	namespace KMaterial { class Material; }
}

namespace KEngines { namespace KObject {
	using namespace KVector;
	using namespace KMatrix;

	//TODO: bindUniform in render function?
	class Object3D {
		friend class Group;

	protected:
		const static Kint A_POSITION; // = 0; a_position
		const static Kint A_TEX_COORD; // = 1; a_tex_coord
		const static Kint A_NORMAL; // = 2; a_normal

		const static std::string U_MODEL_MATRIX; //u_model_matrix
		const static std::string U_NORMAL_MATRIX; //u_normal_matrix

		KBuffer::VertexArray* vao;
		KBuffer::VertexBuffer* ibo;

		KBuffer::VertexBuffer* vbo;
		KBuffer::VertexBuffer* tbo;
		KBuffer::VertexBuffer* nbo;

		KMaterial::Material* material;

		const Object3D* parent;

		vec3 position;
		vec3 scale_size;
		quaternion rotation;

		mat3 normal_matrix;
		mat4 model_matrix;

		const std::string type;

	protected:
		Object3D(const std::string& type);

		virtual void updateMatrix();

		//Note: remember to set parent to nullptr when deleting parent;
		void setParent(const Object3D* object) { parent = object; }

	public:
		virtual ~Object3D();

		const std::string& getType()const { return type; }

		virtual void bind()const;

		virtual void unBind()const;

		virtual void bindUniform(const KRenderer::Shader* shader)const;

		virtual void setMaterial(KMaterial::Material* mat);

		//Remember to bind shader before you render it;
		virtual void render(const KRenderer::Shader* shader = nullptr)const = 0;

		void setPosition(const vec3& v) {
			position = v;
			updateMatrix();
		}

		void setRotation(const quaternion& q) {
			rotation = q;
			updateMatrix();
		}

		void setScale(const vec3& v) {
			scale_size = v;
			updateMatrix();
		}

		void translate(const vec3& v) {
			position += v;
			updateMatrix();
		}

		void rotate(const quaternion& q) {
			rotation *= q;
			updateMatrix();
		}

		void scale(const vec3& v) {
			if (v == vec3(0)) {
				Log::debug("Scale with zero may get valid result!");
			}

			scale_size *= v;
			updateMatrix();
		}

		const vec3& getPostion()const { return position; }

		const vec3& getScale()const { return scale_size; }

		mat3 getNormalMatrix()const {
			if(parent == nullptr) return normal_matrix;
			return normal_matrix * parent->getNormalMatrix();
		}

		mat4 getModelMatrix()const {
			if(parent == nullptr) return model_matrix;
			return parent->getModelMatrix() * model_matrix;
		}
	};
} }

#endif // !KENGINES_OBJECT3D_H

