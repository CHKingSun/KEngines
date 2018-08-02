#include "ModelNode.h"
#include "Mesh.h"
#include "../render/Shader.h"

#include <assimp/scene.h>

namespace KEngines { namespace KObject {
	ModelNode::ModelNode(const aiNode* node, const aiScene* scene, const mat4& transform) {
		model_matrix = transform;
		normal_matrix = model_matrix.transpose().inverse().toMat3();
		model_matrix = transform;

		objects.reserve(node->mNumMeshes);
		for (int i = 0; i < node->mNumMeshes; ++i) {
			dealMesh(scene->mMeshes[node->mMeshes[i]], scene);
		}
	}

	void ModelNode::updateMatrix() {

	}

	void ModelNode::dealMesh(const aiMesh* mesh, const aiScene* scene) {
		auto vertices = new std::vector<Vertex>();
		vertices->reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			aiVector3D vertex = mesh->mVertices[i];
			aiVector3D normal = mesh->mNormals[i];
			aiVector3D tex_coord(0);
			if (mesh->HasTextureCoords(0)) {
				tex_coord = mesh->mTextureCoords[0][i];
			}

			vertices->emplace_back(
				vec3(vertex.x, vertex.y, vertex.z),
				vec3(normal.x, normal.y, normal.z),
				vec2(tex_coord.x, tex_coord.y)
			);
		}

		auto indices = new std::vector<Ksize>();
		indices->reserve(mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];

			indices->emplace_back(face.mIndices[0]);
			indices->emplace_back(face.mIndices[1]);
			indices->emplace_back(face.mIndices[2]);
		}

		this->addObject(new Mesh(vertices, indices));
	}

	void ModelNode::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();

		shader->bindUniformMat3f(U_NORMAL_MATRIX.c_str(), getNormalMatrix());
		shader->bindUniformMat4f(U_MODEL_MATRIX.c_str(), getModelMatrix());
	}

	void ModelNode::render(const KRenderer::Shader* shader /* = nullptr */)const {
		if (shader == nullptr) {
			Log::error("Uniform can not be bind if shader is null in group.");
		}

		bindUniform(shader);
		for (const auto& object : objects) {
			object->render();
		}
	}
} }