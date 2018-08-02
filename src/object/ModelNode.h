//
// Created by KingSun on 2018/08/02
//

#ifndef KENGINES_MODEL_NODE_H
#define KENGINES_MODEL_NODE_H

#include "Group.h"

class aiScene;
class aiNode;
class aiMesh;

namespace KEngines { namespace KObject {
	class ModelNode : public Group {
		friend class Model;

	private:
		void dealMesh(const aiMesh* mesh, const aiScene* scene);

	protected:
		ModelNode(const aiNode* node, const aiScene* scene, const mat4& transform);
		~ModelNode() = default;

		void updateMatrix()override;

	public:
		void bindUniform(const KRenderer::Shader* shader)const override;
		void render(const KRenderer::Shader* shader = nullptr)const override;
	};
} }

#endif // !KENGINES_MODEL_NODE_H
