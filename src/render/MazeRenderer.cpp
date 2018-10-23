#include "MazeRenderer.h"
#include "Shader.h"
#include "../math/vector/vec_default.h"
#include "../camera/FirstCamera.h"
#include "../light/DirectionLight.h"
#include "../material/CubeTexture.h"
#include "../material/ShadowMap.h"
#include "../object/Group.h"
#include "../object/Maze.h"
#include "../object/Sphere.h"
#include "../object/Model.h"

namespace KEngines { namespace KRenderer {
	MazeRenderer::MazeRenderer(const std::string& title, Ksize swidth /* = 1000 */, Ksize sheight /* = 700 */) :
		Renderer(title, swidth, sheight), shader(nullptr), shadow_map(nullptr), camera(nullptr),
		light(nullptr), objects(nullptr), cube_map(nullptr), default_camera(nullptr), camera_change(false) {
		//hideMouse(true);
		glViewport(0, 0, swidth, sheight);

		shader = new Shader(SHADER_PATH + "shadow.vert", SHADER_PATH + "shadow.frag");

		shadow_map = new KMaterial::ShadowMap(1920, 1080);

		camera = new KCamera::FirstCamera(60.f, Kfloat(swidth) / Kfloat(sheight), 0.1f, 1000.f, vec3(0.f, 9.f, 20.f));
		camera->rotateView(quaternion(180.f, UpVector));

		default_camera = new KCamera::FirstCamera(vec3(0.f, 10.f, 0.f), ZeroVector, UpVector);

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
		auto maze = new KObject::Maze();
		camera->setPostion(maze->getStartPosition());
		objects->addObject(maze);

		auto alonne = new KObject::Model(MODEL_PATH + "Alonne/Alonne.obj");
		alonne->scale(vec3(0.3f));
		alonne->setPosition(maze->getStartPosition() + vec3(-1.f, -0.48f, 1.f));
		objects->addObject(alonne);
	}

	MazeRenderer::~MazeRenderer() {
		delete objects;
		delete camera;
		delete default_camera;
		delete light;
		delete cube_map;
		delete shadow_map;
		delete shader;
	}

	void MazeRenderer::exec() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		auto consolas_font = new KObject::Font(RES_PATH + "fonts/Consolas.ttf", 24.f);
		KObject::Font::setViewport(w_size);

		camera->bindUniform(shader);
		light->bindUniform(shader);
		shadow_map->bindDirectionLight(
		{
			vec3(-20.f, -20.f,  1.f), vec3( 20.f, -20.f,  1.f),
			vec3( 20.f,  20.f,  1.f), vec3(-20.f,  20.f,  1.f),
			vec3(-20.f, -20.f, -1.f), vec3( 20.f, -20.f, -1.f),
			vec3( 20.f,  20.f, -1.f), vec3(-20.f,  20.f, -1.f)
		},
		light->getDirection());
		shadow_map->renderShadow(objects, w_size);
		shadow_map->bindShadowTexture(shader);

		Kfloat depth = 0.f;
		Kfloat per_depth = 0.0001f;
		Kfloat last_time = window->getCurrentTime();

		const std::wstring frame_display(L"Frame: ");
		while (!window->closed()) {
			window->clear();

			light->rotate(quaternion(1.f, vec3(0.f, 1.f, 0.f)));
			light->bindDirection(shader);
			shadow_map->bindDirectionLight(
			{
				vec3(-9.f, -9.f,  9.f), vec3(9.f, -9.f,  9.f),
				vec3(9.f,  9.f,  9.f), vec3(-9.f,  9.f,  9.f),
				vec3(-9.f, -9.f, -9.f), vec3(9.f, -9.f, -9.f),
				vec3(9.f,  9.f, -9.f), vec3(-9.f,  9.f, -9.f)
			},
			light->getDirection());
			shadow_map->renderShadow(objects, w_size);
			shadow_map->bindShadowTexture(shader);
			objects->render(shader);

 			glCullFace(GL_FRONT);
 			cube_map->render();
 			glCullFace(GL_BACK);

			consolas_font->renderText(frame_display + std::to_wstring(window->getCurrentFrame()),
				vec3(0.17f, 0.57f, 0.69f), 6, 6);
			consolas_font->renderText(L"Press 'ESC' to display mouse.", vec3(0.17f, 0.57f, 0.69f), 6, w_size.y - 30);
			consolas_font->renderText(L"Press Ctrl + Z to exit.", vec3(0.17f, 0.57f, 0.69f), 6, w_size.y - 60);

			consolas_font->renderText(L"Depth: " + std::to_wstring(depth), vec3(0.17f, 0.57f, 0.69f), 6, w_size.y - 90);
			if (window->getCurrentTime() - last_time >= 0.1f) {
				depth += per_depth;
				if (depth >= 0.008f || depth <= 0.f) per_depth = -per_depth;
				shader->bind();
				shader->bindUniform1f("u_depth", depth);
				last_time = window->getCurrentTime();
			}

			move();

			if (camera_change) {
				default_camera->bindUniform(shader);
				cube_map->bindMatrix(default_camera);
			}
			else {
				camera->bindUniform(shader);
				cube_map->bindMatrix(camera);
			}

			window->update();
		}

		delete consolas_font;
	}

	void MazeRenderer::resize(Kint w, Kint h) {
		Renderer::resize(w, h);

		if (w != 0 && h != 0) {
			camera->setPerspectiveProjection(60.f, Kfloat(w) / Kfloat(h), 0.1, 1000.f);
			camera->bindUniform(shader);
			cube_map->bindMatrix(camera);
		}
	}

	void MazeRenderer::keyEvent(Kint key, Kint action) {
		keys[key] = (action != GLFW_RELEASE);
		if (keys[GLFW_KEY_ESCAPE]) {
			hideMouse(false);
		}

		if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_Z]) {
			window->closeWindow();
		}

		if (keys[GLFW_KEY_V]) camera_change = !camera_change;
	}

	void MazeRenderer::cursorEvent(Kdouble xpos, Kdouble ypos) {
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

	void MazeRenderer::mouseEvent(Kint button, Kint action) {
		Renderer::mouseEvent(button, action);

		if (window->focused() && mouse[GLFW_MOUSE_BUTTON_LEFT]) {
			hideMouse(true);
		}
	}

	void MazeRenderer::move()const {
		static const vec3 speed = vec3(0.03f, 0.f, 0.03f);

		bool moved = false;
		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
			camera->translate(camera->getDirection(BACK) *= speed);
			moved = true;
		}
		if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
			camera->translate(camera->getDirection(FORWARD) *= speed);
			moved = true;
		}
		if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
			camera->translate(camera->getDirection(RIGHT) *= speed);
			moved = true;
		}
		if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
			camera->translate(camera->getDirection(LEFT) *= speed);
			moved = true;
		}
		if (moved) camera->bindUniform(shader);
	}
} }