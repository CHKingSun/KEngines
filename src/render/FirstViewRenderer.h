//
// Created by KingSun on 2018/08/31
//

#ifndef KENGINES_FIRST_VIEW_RENDERER_H
#define KENGINES_FIRST_VIEW_RENDERER_H

#include "Renderer.h"

namespace KEngines {
	namespace KRenderer { class Shader; }
	namespace KCamera { class FirstCamera; }
	namespace KLight { class DirectionLight; }
	namespace KObject {
		class Object3D;
		class Group;
	}
	namespace KMaterial { 
		class CubeTexture;
		class ShadowMap;
	}
}

namespace KEngines { namespace KRenderer {
		class FirstViewRenderer : public Renderer {
		private:
			Shader* shader;
			KCamera::FirstCamera* camera;
			KLight::DirectionLight* light;
			KMaterial::CubeTexture* cube_map;
			KMaterial::ShadowMap* shadow_map;

			KObject::Group* objects;

		protected:
			void cursorEvent(Kdouble xpos, Kdouble ypos)override;

			void keyEvent(Kint key, Kint action)override;

			void mouseEvent(Kint button, Kint action)override;

			void move()const;

		public:
			FirstViewRenderer(const std::string& title, Ksize swidth = 1000, Ksize sheight = 700);
			~FirstViewRenderer();

			void addObject(KObject::Object3D* object);
			void deleteObject(Ksize index);

			void exec()override;

			void resize(Kint w, Kint h)override;
		};
} }

#endif // !KENGINES_FIRST_VIEW_RENDERER_H

