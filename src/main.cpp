#include <iostream>

//#define TEST
#ifdef TEST

#include "KHeader.h"
#include "math/vector/vec2.h"
#include "math/vector/vec3.h"
#include "math/vector/vec4.h"
#include "math/matrix/mat3.h"
#include "math/matrix/mat4.h"
#include "math/matrix/quaternion.h"
#include "math/function.h"
#include "math/transform.h"
#include "render/Shader.h"
#include "Log.h"

void test() {
	using namespace KEngines;
	using namespace KEngines::KVector;
	using namespace KEngines::KMatrix;
	using namespace KEngines::KFunction;

	vec2 v = (vec2(0.f) *= 2);
	std::cout << "Hello KEngines!" << std::endl;
	std::cout << vec2().normalize() << std::endl;
	std::cout << (vec2(2.f) * 3) << std::endl;
	//std::cout << (3 * vec2(2.f)).normalize() << std::endl;
	std::cout << normalize(vec4(2.f)) << std::endl;
	std::cout << vec3::cross(vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 1.f)) << std::endl;

	std::cout << reflect(vec3(1, 0, 0), vec3(0, 1, 0)) << std::endl;
	std::cout << refract(vec3(1, 1, 0), vec3(0, 1, 0), 1.2f) << std::endl;

	std::cout << (mat4(1) * vec4(2, 2, 2, 3)) << std::endl;
	std::cout << mat3(1, 2, 3, 4, 5, 6, 7, 8, 9).transpose() * 3 << std::endl;
	std::cout << mat3(1, 2, 3, 4, 5, 6, 7, 8, 8).determinant() << std::endl;
	std::cout << 3 * mat3(1, 2, 3, 4, 7, 6, 7, 8, 8).inverse() << std::endl << std::endl;

	std::cout << translate(vec3(5, 7, 0)) << std::endl << std::endl;
	std::cout << rotate(90.f, vec3(1, 0, 0)) << std::endl << std::endl;
	std::cout << scale(vec3(1, 2, 3)) << std::endl << std::endl;

	std::cout << lookAt(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 1, 0)) << std::endl << std::endl;

	std::cout << frustum(3.f, 4.f, 5.f, 6.f, 7.f, 8.f) << std::endl << std::endl;

	std::cout << perspective(60.f, 1.f, 0.1f, 100.f) << std::endl << std::endl;

	std::cout << unProject(vec3(1.f), translate(vec3(0, 1, 0)),
		perspective(60.f, 1.f, 0.1f, 100.f), ivec4(0, 0, 100, 100))
		<< std::endl << std::endl;

	std::cout << (-quaternion(90.f, vec3(1, 0, 0)) * vec3(2, 2, 0)) << std::endl;

	Log::d("Hello World! %d, %lf\n", 1, 2.f);
	Log::debug("Hello World! ", vec3(), std::setbase(8), 20);
	Log::debug("Hello template!");

	std::cin.get();
}

#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include <clocale>
#include <locale>

#include "KHeader.h"
#include "Log.h"

#include "render/Shader.h"
#include "render/Window.h"
#include "buffer/VertexArray.h"
#include "math/transform.h"
#include "object/Font.h"
#include "object/Plane.h"
#include "object/Group.h"
#include "util/StringUtil.h"
#include "buffer/FrameBuffer.h"
#include "object/Model.h"

using namespace KEngines;
using namespace KEngines::KFunction;
using namespace KEngines::KVector;
using namespace KEngines::KBuffer;
using namespace KEngines::KRenderer;
using namespace KEngines::KObject;
using namespace KEngines::KUtil;

void initLocale() {
	Log::info("Local locale: ", std::locale().name().c_str());

	std::setlocale(LC_CTYPE, "");
}

int main() {

	initLocale();

	auto window = new Window("KEngines");

	auto group = new Group(3);
	auto plane = new Plane(10.f, 10.f, 10, 10);
	plane->rotate(quaternion(30.f, vec3(-1.f, 0.f, 0.f)));
	group->addObject(plane);

	plane = new Plane(5.f, 5.f, 10, 10);
	plane->translate(vec3(2.f, 0.f, 0.f));
	group->addObject(plane);

	plane = new Plane(6.f, 6.f, 10, 10);
	plane->scale(vec3(1.f, 2.f, 3.f));
	plane->rotate(quaternion(60, vec3(0.f, 1.f, 0.f)));
	group->addObject(plane);
	plane = nullptr;

	auto model = new Model(MODEL_PATH + "cellrain.obj");
	model->scale(vec3(0.003f));

	auto shader = new Shader(SHADER_PATH + "test.vert", SHADER_PATH + "test.frag");
	shader->bind();
	shader->bindUniformMat4f("u_proj",
		perspective(60.f, 1.f, 0.1f, 1000.f)
		//ortho(-12.f, 12.f, -12.f, 12.f, -12.f, 12.f)
		* lookAt(vec3(0.f, 0.f, 15.f), vec3(), vec3(0.f, 1.f, 0.f))
	);
	auto test_font = new Font();
	delete test_font;

	auto consolas_font = new Font(RES_PATH + "fonts/Consolas.ttf", 24.f);
	auto kai_font = new Font(RES_PATH + "fonts/STXINGKAI.TTF", 24.f);
	Font::setViewport(window->getWindowSize());

	kai_font->loadText(L"你好，世界！");
	kai_font->loadText(L"こんにちは，世界！");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ivec2 w_size = window->getWindowSize();
	auto frame_buffer = new FrameBuffer(w_size.x, w_size.y);
	frame_buffer->setOpacity(0.36f);

	const std::wstring frame_display(L"Frame: ");
	while (!window->closed()) {
		window->clear(); //Clear color buffer and depth buffer.
		frame_buffer->begin();

		group->rotate(quaternion(1.f, vec3(0.f, 0.f, 1.f)));
		group->render(shader);

		model->render(shader);

		//Render.
		consolas_font->renderText(frame_display + std::to_wstring(window->getCurrentFrame()),
								  vec3(0.17f, 0.57f, 0.69f), 6, 6);

		kai_font->renderText(L"你好，世界！", vec3(0.17f, 0.57f, 0.69f), 240, 120);
		kai_font->renderText(L"こんにちは，世界！", vec3(0.17f, 0.57f, 0.69f), 300, 150);
		kai_font->renderText(L"Hello, World!", vec3(0.17f, 0.57f, 0.69f), 360, 180);

		frame_buffer->end();
		frame_buffer->render();

		window->update();
	}

	delete frame_buffer;

	delete consolas_font;
	delete kai_font;

	delete group;
	delete model;
	delete shader;

	delete window;

	return 0;
}