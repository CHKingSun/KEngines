#include "Model.h"
#include "Mesh.h"
#include "ModelNode.h"
#include "../render/Shader.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace KEngines { namespace KObject {
	Model::Model(const std::string& path) : Group(0, "Model") {
		openFile(path);
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

		dealNode(scene->mRootNode, scene, aiMatrix4x4());

		aiReleaseImport(scene);
		return true;
	}

	void Model::dealNode(const aiNode* node, const aiScene* scene, const aiMatrix4x4& transform) {
		if (node == nullptr) return;

		aiMatrix4x4 new_transform = node->mTransformation * transform;

		this->addObject(new ModelNode(node, scene, aiMatrixToMatrix(new_transform), dir));

		for (int i = 0; i < node->mNumChildren; ++i) {
			dealNode(node->mChildren[i], scene, new_transform);
		}
	}

	mat4 Model::aiMatrixToMatrix(const aiMatrix4x4& m) {
		return mat4(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		);
	}
} }