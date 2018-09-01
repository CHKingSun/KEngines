#include "FirstViewRenderer.h"
#include "Shader.h"
#include "../math/vector/vec_default.h"
#include "../camera/FirstCamera.h"
#include "../light/DirectionLight.h"
#include "../material/CubeTexture.h"
#include "../object/Group.h"

namespace KEngines { namespace KRenderer {
	FirstViewRenderer::FirstViewRenderer(const std::string& title, Ksize swidth /* = 1000 */, Ksize sheight /* = 700 */) :
		Renderer(title, swidth, sheight), shader(nullptr), camera(nullptr),
		light(nullptr), objects(nullptr), cube_map(nullptr) {
		hideMouse(true);
		glViewport(0, 0, swidth, sheight);

		shader = new Shader(SHADER_PATH + "light.vert", SHADER_PATH + "light.frag");

		camera = new KCamera::FirstCamera(60.f, Kfloat(swidth) / Kfloat(sheight), 0.1f, 1000.f, vec3(0.f, 9.f, 20.f));

		light = new KLight::DirectionLight(vec3(-3.f, -3.f, -1.f));

		cube_map = new KMaterial::CubeTexture({
			IMAGE_PATH + "darkskies/darkskies_rt.png",
			IMAGE_PATH + "darkskies/darkskies_lf.png",
			IMAGE_PATH + "darkskies/darkskies_up.png",
			IMAGE_PATH + "darkskies/darkskies_dn.png",
			IMAGE_PATH + "darkskies/darkskies_bk.png",
			IMAGE_PATH + "darkskies/darkskies_ft.png"
		});
		cube_map->bindMatrix(camera);

		objects = new KObject::Group();
	}

	FirstViewRenderer::~FirstViewRenderer() {
		delete objects;
		delete camera;
		delete light;
		delete cube_map;
		delete shader;
	}

	void FirstViewRenderer::addObject(KObject::Object3D* object) {
		objects->addObject(object);
	}

	void FirstViewRenderer::deleteObject(Ksize index) {
		objects->deleteObject(index);
	}

	void FirstViewRenderer::exec() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);

		auto consolas_font = new KObject::Font(RES_PATH + "fonts/Consolas.ttf", 24.f);
		auto kai_font = new KObject::Font(RES_PATH + "fonts/STXINGKAI.TTF", 24.f);
		KObject::Font::setViewport(w_size);

		kai_font->loadText(L"你好，世界！");
		kai_font->loadText(L"こんにちは，世界！");

		camera->bindUniform(shader);
		light->bindUniform(shader);

		const std::wstring frame_display(L"Frame: ");
		while (!window->closed()) {
			window->clear();

			//light->rotate(quaternion(1.f, vec3(0.f, 1.f, 0.f)));
			//light->bindDirection(shader);
			objects->render(shader);

			cube_map->render();

			consolas_font->renderText(frame_display + std::to_wstring(window->getCurrentFrame()),
				vec3(0.17f, 0.57f, 0.69f), 6, 6);
			consolas_font->renderText(L"Press 'ESC' to display mouse.", vec3(0.17f, 0.57f, 0.69f), 6, w_size.y - 30);
			consolas_font->renderText(L"Press Ctrl + Z to exit.", vec3(0.17f, 0.57f, 0.69f), 6, w_size.y - 60);

			kai_font->renderText(L"你好，世界！", vec3(0.17f, 0.57f, 0.69f), 240, 120);
			kai_font->renderText(L"こんにちは，世界！", vec3(0.17f, 0.57f, 0.69f), 300, 150);
			kai_font->renderText(L"Hello, World!", vec3(0.17f, 0.57f, 0.69f), 360, 180);

			window->update();
		}

		delete consolas_font;
		delete kai_font;
	}

	void FirstViewRenderer::resize(Kint w, Kint h) {
		Renderer::resize(w, h);

		if (w != 0 && h != 0) {
			camera->setPerspectiveProjection(60.f, Kfloat(w) / Kfloat(h), 0.1, 1000.f);
			camera->bindUniform(shader);
			cube_map->bindMatrix(camera);
		}
	}

	void FirstViewRenderer::keyEvent(Kint key, Kint action) {
		keys[key] = (action != GLFW_RELEASE);
		if (keys[GLFW_KEY_ESCAPE]) {
			hideMouse(false);
		}

		if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_Z]) {
			window->closeWindow();
		}

		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
			camera->translate(camera->getDirection(BACK));
			camera->bindUniform(shader);
		}
		else if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
			camera->translate(camera->getDirection(FORWARD));
			camera->bindUniform(shader);
		}
		else if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
			camera->translate(camera->getDirection(RIGHT));
			camera->bindUniform(shader);
		}
		else if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
			camera->translate(camera->getDirection(LEFT));
			camera->bindUniform(shader);
		}
	}

	void FirstViewRenderer::cursorEvent(Kdouble xpos, Kdouble ypos) {
 		if (mouse_hide) {
 			Kfloat x = xpos - mouse_pos.x;
 			Kfloat y = ypos - mouse_pos.y;
 			Kfloat z = std::sqrt(w_size.dot(w_size) - x * x - y * y);
 			camera->rotateToDirection(vec3(x, y, z));
 			camera->bindUniform(shader);
 			cube_map->bindMatrix(camera);
 		}

		Renderer::cursorEvent(xpos, ypos);
	}

	void FirstViewRenderer::mouseEvent(Kint button, Kint action) {
		Renderer::mouseEvent(button, action);

		if (window->focused() && mouse[GLFW_MOUSE_BUTTON_LEFT]) {
			hideMouse(true);
		}
	} 
} }