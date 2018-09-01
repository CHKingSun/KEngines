//
// Created by KingSun on 2018/08/01
//

#ifndef KEGINES_VIEW_RENDERER_H
#define KEGINES_VIEW_RENDERER_H

#include "Renderer.h"

namespace KEngines {
	namespace KRenderer { class Shader; }
	namespace KCamera { class Camera; }
	namespace KLight {
		class BasicLight;
		class DirectionLight;
		class PointLight;
		class SpotLight;
	}
	namespace KObject {
		class Object3D;
		class Group;
	}
	namespace KMaterial { class CubeTexture; }
}

namespace KEngines { namespace KRenderer {
	class ViewRenderer : public Renderer {
	private:
		Shader* shader;
		KCamera::Camera* camera;
		KLight::BasicLight* light;
		KLight::DirectionLight* e_light;
		KMaterial::CubeTexture* cube_map;

		KObject::Group* objects;

	protected:
		void mouseWheelEvent(Kdouble yoffset)override;

		void cursorEvent(Kdouble xpos, Kdouble ypos)override;

	public:
		ViewRenderer(const std::string& title, Ksize swidth = 1000, Ksize sheight = 700);
		~ViewRenderer();

		void addObject(KObject::Object3D* object);
		void deleteObject(Ksize index);

		void exec()override;

		void resize(Kint w, Kint h)override;
	};
} }

#endif // !KEGINES_VIEW_RENDERER_H
