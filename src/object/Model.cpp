#include "Model.h"
#include "Mesh.h"
#include "../render/Shader.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace KEngines { namespace KObject {
	Model::Model(const std::string& path) : Group(0, "Model") {
		openFile(path);
	}

	Model::~Model() {

	}

	Kboolean Model::openFile(const std::string& path) {
		//Note: scene will be cleaned up by the importer destructor.
		const aiScene *scene = aiImportFile(path.c_str(),
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
			| aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_FixInfacingNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			Log::error("Error in loading file by assimp: ", aiGetErrorString());
			return false;
		}

		dir = path.substr(0, path.find_last_of('/') + 1);
		filename = path.substr(path.find_last_of('/') + 1, path.size());
		type = filename.substr(filename.find_last_of('.') + 1, filename.size());

		//Note: ignore the aiNode at present.
		objects.reserve(scene->mNumMeshes);
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			dealMesh(scene->mMeshes[i], scene);
		}

		aiReleaseImport(scene);
		return true;
	}

	void Model::dealMesh(const aiMesh* mesh, const aiScene* scene) {
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

	void Model::bindUniform(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();

		shader->bindUniformMat3f(U_NORMAL_MATRIX, getNormalMatrix());
		shader->bindUniformMat4f(U_MODEL_MATRIX, getModelMatrix());
	}

	void Model::render(const KRenderer::Shader* shader /* = nullptr */)const {
		if (shader == nullptr) {
			Log::error("Uniform can not be bind if shader is null in group.");
		}

		bindUniform(shader);
		for (const auto& object : objects) {
			object->render();
		}
	}
} }