//
// Created by KingSun on 2018/07/24
//

#ifndef KENGINES_GROUP_H
#define KENGINES_GROUP_H

#include <vector>
#include "Object3D.h"

namespace KEngines { namespace KObject {

	//TODO: sort function(need Object3D add center and bound parameter)
	//TODO: how to deal with group uniform in shader?
	class Group : public Object3D {
	private:
		std::vector<Object3D*> objects;
		Ksize n_objects;

	public:
		Group(Ksize reserve_size = 0);
		virtual ~Group();

		//TODO: smater pointer?
		Kboolean addObject(Object3D* object);
		Kboolean deleteObject(Ksize index);
		Object3D* getObject(Ksize index)const;

		Ksize size()const { return n_objects; }

		virtual void bindUniform(const KRenderer::Shader* shader)const override {};
		virtual void render(const KRenderer::Shader* shader = nullptr)const override;
	};
} }

#endif // !KENGINES_GROUP_H
