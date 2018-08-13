#include "ViewRenderer.h"
#include "Shader.h"
#include "../camera/Camera.h"
#include "../light/DirectionLight.h"
#include "../light/SpotLight.h"
#include "../light/PointLight.h"
#include "../object/Group.h"

namespace KEngines { namespace KRenderer {
	ViewRenderer::ViewRenderer(const std::string& title, Ksize swidth /* = 1000 */, Ksize sheight /* = 700 */) :
		Renderer(title, swidth, sheight), shader(nullptr), camera(nullptr),
		light(nullptr), e_light(nullptr), objects(nullptr) {
		glViewport(0, 0, swidth, sheight);

		//shader = new Shader(SHADER_PATH + "test.vert", SHADER_PATH + "test.frag");
		//shader = new Shader(SHADER_PATH + "material.vert", SHADER_PATH + "material.frag");
		shader = new Shader(SHADER_PATH + "light.vert", SHADER_PATH + "light.frag");

		camera = new KCamera::Camera(60.f, Kfloat(swidth) / Kfloat(sheight), 0.1f, 1000.f, vec3(0.f, 18.f, 20.f));
		camera->rotateView(quaternion(30.f, vec3(-1.f, 0.f, 0.f)));

		light = new KLight::SpotLight(vec3(0.f, 12.f, 6.f), vec3(0.f, -1.f, -1.f));
		//light = new KLight::PointLight(vec3(0.f, 12.f, 6.f));

		e_light = new KLight::DirectionLight(vec3(-3.f, -3.f, -1.f));

		objects = new KObject::Group();
	}

	ViewRenderer::~ViewRenderer() {
		delete objects;
		delete camera;
		delete light;
		delete e_light;
		delete shader;
	}

	void ViewRenderer::addObject(KObject::Object3D* object) {
		objects->addObject(object);
	}

	void ViewRenderer::deleteObject(Ksize index) {
		objects->deleteObject(index);
	}

	void ViewRenderer::exec() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto consolas_font = new KObject::Font(RES_PATH + "fonts/Consolas.ttf", 24.f);
		auto kai_font = new KObject::Font(RES_PATH + "fonts/STXINGKAI.TTF", 24.f);
		KObject::Font::setViewport(window->getWindowSize());

		kai_font->loadText(L"低挫��弊順��");
		kai_font->loadText(L"こんにちは��弊順��");

		camera->bindUnifrom(shader);
		light->bindUniform(shader);
		e_light->bindUniform(shader);

		const std::wstring frame_display(L"Frame: ");
		while (!window->closed()) {
			window->clear();

			e_light->rotate(quaternion(1.f, vec3(0.f, 1.f, 0.f)));
			e_light->bindDirection(shader);

			objects->render(shader);

			consolas_font->renderText(frame_display + std::to_wstring(window->getCurrentFrame()),
				vec3(0.17f, 0.57f, 0.69f), 6, 6);

			kai_font->renderText(L"低挫��弊順��", vec3(0.17f, 0.57f, 0.69f), 240, 120);
			kai_font->renderText(L"こんにちは��弊順��", vec3(0.17f, 0.57f, 0.69f), 300, 150);
			kai_font->renderText(L"Hello, World!", vec3(0.17f, 0.57f, 0.69f), 360, 180);

			window->update();
		}

		delete consolas_font;
		delete kai_font;
	}

	void ViewRenderer::resize(Kint w, Kint h) {
		Renderer::resize(w, h);

		if (w != 0 && h != 0) {
			camera->setPerspectiveProjection(60.f, Kfloat(w) / Kfloat(h), 0.1, 1000.f);
			camera->bindUnifrom(shader);
		}
	}

	void ViewRenderer::mouseWheelEvent(Kdouble yoffset) {
		if (yoffset > 0 && objects->getScale().x < 10.f) {
			objects->scale(vec3(1.2f));
		}
		else if (yoffset < 0 && objects->getScale().x > 0.1f) {
			objects->scale(vec3(0.8f));
		}
	}

	void ViewRenderer::cursorEvent(Kdouble xpos, Kdouble ypos) {
		if (mouse[GLFW_MOUSE_BUTTON_LEFT]) {
			if (abs(xpos - mouse_pos.x) > 6.f) {
				static const vec3 up_vector(0.0f, 1.0f, 0.0f);
				camera->rotateCamera(quaternion(atan((mouse_pos.x - xpos) / 2.0) * 3.0f, up_vector));
				camera->bindUnifrom(shader);
			}
			//if (abs(ypos - mouse_pos.y) > 6.f) {
			//	static const vec3 forwrad_vector(1.0f, 0.0f, 0.0f);
			//	camera->rotateCamera(quaternion(atan((mouse_pos.y - ypos) / 2.0) * 3.0f, forwrad_vector));
			//	camera->bindUnifrom(shader);
			//}
		}

		Renderer::cursorEvent(xpos, ypos);
	}
} }