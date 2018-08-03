//
// Created by KingSun on 2018/08/02
//

#ifndef KENGINES_MODEL_NODE_H
#define KENGINES_MODEL_NODE_H

#include "Group.h"

namespace KEngines { namespace KMaterial {
	class Material;
	class Texture;
} }

class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;
enum aiTextureType;

namespace KEngines { namespace KObject {
	class ModelNode : public Group {
		friend class Model;

	private:
		std::string dir;

	private:
		void dealMesh(const aiMesh* mesh, const aiScene* scene);

		KMaterial::Material* dealMaterial(const aiMaterial* mat)const;

		void dealTexture(const aiMaterial *mat, aiTextureType type, std::vector<KMaterial::Texture*>& tex)const;

	protected:
		ModelNode(const aiNode* node, const aiScene* scene, const mat4& transform, const std::string& dir);
		~ModelNode() = default;

		void updateMatrix()override;

	public:
		void bindUniform(const KRenderer::Shader* shader)const override;
		void render(const KRenderer::Shader* shader = nullptr)const override;
	};
} }

#endif // !KENGINES_MODEL_NODE_H
