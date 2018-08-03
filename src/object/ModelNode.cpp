#include "ModelNode.h"
#include "Mesh.h"
#include "../render/Shader.h"
#include "../material/Material.h"
#include "../material/Texture.h"

#include <assimp/scene.h>

namespace KEngines { namespace KObject {
	ModelNode::ModelNode(const aiNode* node, const aiScene* scene, const mat4& transform,
		const std::string& dir) : dir(dir) {
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

		this->addObject(new Mesh(vertices, indices, dealMaterial(scene->mMaterials[mesh->mMaterialIndex])));
	}

	KMaterial::Material* ModelNode::dealMaterial(const aiMaterial* mat)const {
		aiColor3D ambient, diffuse, specular;
		Kfloat shininess, opacity;

		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		mat->Get(AI_MATKEY_SHININESS, shininess);
		mat->Get(AI_MATKEY_OPACITY, opacity);
		opacity = opacity == 0.f ? 1.f : opacity;

		std::vector<KMaterial::Texture*> tex;
		Ksize count = mat->GetTextureCount(aiTextureType_AMBIENT)
					+ mat->GetTextureCount(aiTextureType_DIFFUSE)
					+ mat->GetTextureCount(aiTextureType_SPECULAR);
		if (count > 0) {
			tex.reserve(count);
			dealTexture(mat, aiTextureType_AMBIENT, tex);
			dealTexture(mat, aiTextureType_DIFFUSE, tex);
			dealTexture(mat, aiTextureType_SPECULAR, tex);
		}

		return new KMaterial::Material(tex, 
			vec3(ambient.r, ambient.g, ambient.b),
			vec3(diffuse.r, diffuse.g, diffuse.b),
			vec3(specular.r, specular.g, specular.b),
			shininess, opacity
		);
	}

	void ModelNode::dealTexture(const aiMaterial *mat, aiTextureType type,
		std::vector<KMaterial::Texture *>& tex)const {
		KMaterial::TextureType tex_type;
		switch (type) {
		case aiTextureType_AMBIENT:
			tex_type = KMaterial::AMBIENT;
			break;

		case aiTextureType_DIFFUSE:
			tex_type = KMaterial::DIFFUSE;
			break;

		case aiTextureType_SPECULAR:
			tex_type = KMaterial::SPECULAR;
			break;
		}

		for (int i = 0; i < mat->GetTextureCount(type); ++i) {
			aiString path;
			if (mat->GetTexture(type, i, &path) == aiReturn_FAILURE) {
				Log::error("Read texture msg failed!");
				continue;
			}

			//Log::debug(path.C_Str());
			tex.emplace_back(new KMaterial::Texture(dir + path.C_Str(), tex_type));
		}
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
			object->bindUniform(shader);
			object->render();
		}
	}
} }