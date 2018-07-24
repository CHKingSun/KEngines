#include "Group.h"
#include "../render/Shader.h"

namespace KEngines { namespace KObject {
	Group::Group(Ksize reserve_size /* = 0 */): Object3D("Group"), 
		objects(), n_objects(0) {
		if (reserve_size != 0) objects.reserve(reserve_size);
	}

	Group::~Group() {
		for (auto& object : objects) {
			delete object;
			object = nullptr;
		}
	}

	Kboolean Group::addObject(Object3D* object) {
		if (object == nullptr) {
			Log::error("Object added is null pointer!");
			return false;
		}

		objects.emplace_back(object);
		object->setParent(this);
		++n_objects;

		return true;
	}

	Kboolean Group::deleteObject(Ksize index) {
		if (index >= n_objects) return false;

		auto it = objects.begin();
		std::advance(it, index);
		delete (*it); (*it) = nullptr;
		objects.erase(it);
		--n_objects;

		return true;
	}

	Object3D* Group::getObject(Ksize index)const {
		if (index >= n_objects) return nullptr;

		return objects[index];
	}

	void Group::render(const KRenderer::Shader* shader /* = nullptr */)const {
		if (shader == nullptr) {
			Log::error("Uniform can not be bind if shader is null in group.");
		}

		for (const auto& object : objects) {
			object->bindUniform(shader);
			object->render();
		}
	}
} }