//
// Created by KingSun on 2018/07/25
//

#ifndef KENGINES_MODEL_H
#define KENGINES_MODEL_H

#include <assimp/matrix4x4.h>

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
		mat4 aiMatrixToMatrix(const aiMatrix4x4& m);
		void dealNode(const aiNode* node, const aiScene* scene, const aiMatrix4x4& transform);

	public:
		Model(const std::string& path);
		~Model() = default;
	};
} }

#endif // !KENGINES_MODEL_H
