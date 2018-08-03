#include "Object3D.h"
#include "../render/Shader.h"
#include "../buffer/VertexArray.h"
#include "../material/Material.h"

namespace KEngines { namespace KObject {
	const Kint Object3D::A_POSITION = 0; //a_position
	const Kint Object3D::A_TEX_COORD = 1; //a_tex_coord
	const Kint Object3D::A_NORMAL = 2; //a_normal

	const std::string Object3D::U_MODEL_MATRIX{ "u_model_matrix" };
	const std::string Object3D::U_NORMAL_MATRIX{ "u_normal_matrix" };

	Object3D::Object3D(const std::string& type): type(type), parent(nullptr),
		vao(nullptr), ibo(nullptr), vbo(nullptr), tbo(nullptr), nbo(nullptr),
		position(), rotation(), scale_size(1.f), normal_matrix(), model_matrix(),
		material(nullptr) {}

	Object3D::~Object3D() {
		Log::info("Delete type: ", type);
		delete vao;
		delete ibo;
		delete vbo;
		delete tbo;
		delete nbo;

		delete material;
	}

	void Object3D::updateMatrix() {
		mat3 tmp = rotation.toMat3();
		normal_matrix = mat3(vec3(
			1.f / scale_size.x, 1.f / scale_size.y, 1.f / scale_size.z
		)) *= tmp;

		tmp *= mat3(scale_size);
		model_matrix = mat4(
			vec4(tmp[0], position.x),
			vec4(tmp[1], position.y),
			vec4(tmp[2], position.z),
			vec4(0.f, 0.f, 0.f, 1.f)
		);
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
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();

		shader->bindUniformMat3f(U_NORMAL_MATRIX.c_str(), getNormalMatrix());
		shader->bindUniformMat4f(U_MODEL_MATRIX.c_str(), getModelMatrix());

		if (material != nullptr) material->bindUniform(shader);
	}

	void Object3D::setMaterial(KMaterial::Material* mat) {
		if (mat == nullptr) {
			Log::info("Note: you set material to nullptr.");
		}

		delete material;
		material = mat;
	}
} }