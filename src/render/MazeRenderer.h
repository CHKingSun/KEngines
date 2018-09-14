//
// Created by KingSun on 2018/09/03
//

#ifndef KENGINES_MAZE_RENDERER_H
#define KENGINES_MAZE_RENDERER_H

#include "Renderer.h"

namespace KEngines {
	namespace KRenderer { class Shader; }
	namespace KCamera { class FirstCamera; }
	namespace KLight { class DirectionLight; }
	namespace KObject { class Group; }
	namespace KMaterial {
		class ShadowMap;
		class CubeTexture;
	}
}

namespace KEngines { namespace KRenderer {
	class MazeRenderer : public Renderer {
	private:
		Shader* shader;
		KMaterial::ShadowMap* shadow_map;
		KCamera::FirstCamera* camera;
		KCamera::FirstCamera* default_camera;
		KLight::DirectionLight* light;
		KMaterial::CubeTexture* cube_map;

		KObject::Group* objects;

		Kboolean camera_change;

	protected:
		void cursorEvent(Kdouble xpos, Kdouble ypos)override;

		void keyEvent(Kint key, Kint action)override;

		void mouseEvent(Kint button, Kint action)override;

		void move()const;

	public:
		MazeRenderer(const std::string& title, Ksize swidth = 1000, Ksize sheight = 700);
		~MazeRenderer();

		void exec()override;

		void resize(Kint w, Kint h)override;
	};
} }

#endif // !KENGINES_MAZE_RENDERER_H
