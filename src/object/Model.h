//
// Created by KingSun on 2018/07/25
//

#ifndef KENGINES_MODEL_H
#define KENGINES_MODEL_H

#include "Group.h"

class aiScene;
class aiNode;
class aiMesh;

namespace KEngines { namespace KObject {
	//A class to deal with file imported by Assimp;
	class Model : public Group {
	private:
		std::string dir;
		std::string filename;
		std::string type;

		Kboolean openFile(const std::string& path);
		//void dealNode(const aiNode* node, const aiScene* scene);
		void dealMesh(const aiMesh* mesh, const aiScene* scene);

	public:
		Model(const std::string& path);
		~Model();

		void bindUniform(const KRenderer::Shader* shader)const override;
		void render(const KRenderer::Shader* shader = nullptr)const override;
	};
} }

#endif // !KENGINES_MODEL_H
