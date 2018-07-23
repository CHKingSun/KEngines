#include "Object3D.h"
#include "../render/Shader.h"
#include "../buffer/VertexArray.h"

namespace KEngines { namespace KObject {
	const Kint Object3D::A_POSITION = 0; //a_position
	const Kint Object3D::A_TEX_COORD = 1; //a_tex_coord
	const Kint Object3D::A_NORMAL = 2; //a_normal

	const char Object3D::U_POSITION[]{ "u_position" };
	const char Object3D::U_ROTATION[]{ "u_rotation" };
	const char Object3D::U_SCALE[]{ "u_scale" };

	Object3D::Object3D(const std::string& type): type(type),
		vao(nullptr), ibo(nullptr), vbo(nullptr), tbo(nullptr), nbo(nullptr),
		position(vec3(0.f)), rotation(quaternion()), scale_size(vec3(1.0f)) {}

	Object3D::~Object3D() {
		Log::info("Delete type: ", type);
		delete vao;
		delete ibo;
		delete vbo;
		delete tbo;
		delete nbo;
	}

	void Object3D::bind()const {
		if (vao != nullptr) {
			vao->bind();
			vao->enableVertexArray();
		}
		if (ibo != nullptr) ibo->bind();
	}

	void Object3D::unBind()const {
		if (vao != nullptr) {
			vao->unBind();
			vao->disableVertexArray();
		}
		if (ibo != nullptr) ibo->unBind();
	}

	void Object3D::bindUniform(const KRenderer::Shader* shader)const {
		shader->bind();

		shader->bindUniform3f(U_POSITION, position);
		shader->bindUniformMat3f(U_ROTATION, rotation.toMat3());
		shader->bindUniform3f(U_SCALE, scale_size);
	}
} }