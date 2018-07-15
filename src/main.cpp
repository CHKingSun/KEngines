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
#include "buffer/VertexArray.h"
#include "math/transform.h"
#include "object/Font.h"
#include "util/StringUtil.h"

using namespace KEngines;
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

	if (glfwInit() == GLFW_FALSE) {
		Log::error("GLFW initialized failed!");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef KDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // comment this line in a release build!
														// enable OpenGL debug context if context allows for debug context
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif

	ivec2 s_size(1200, 900);
	GLFWwindow* window = glfwCreateWindow(s_size.x, s_size.y, "KEngines", nullptr, nullptr);
	if (!window) {
		Log::error("Create window failed!");
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		Log::error("GLEW initialized failed!");
		return -3;
	}

	Log::info("OpenGL Version: ", glGetString(GL_VERSION));

	glViewport(0, 0, s_size.x, s_size.y);
	Font consolas_font(RES_PATH + "fonts/Consolas.ttf", 24.f);
	Font kai_font(RES_PATH + "fonts/STXINGKAI.TTF", 24.f);
	Font::setViewport(s_size);
	kai_font.loadText(stringToWString("你好，世界！"));
	kai_font.loadText(stringToWString("こんにちは，世界！"));

	glClearColor(0.27f, 0.27f, 0.27f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const std::wstring frame_display(stringToWString("Frame: "));
	Kfloat last_time = glfwGetTime();
	Kuint frame_count = 0;
	Kuint frame_current = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Kfloat now_time = glfwGetTime();
		++frame_count;
		if (now_time - last_time >= 0.2f) {
			last_time = now_time;
			frame_current = frame_count * 5;
			frame_count = 0;
		}
		consolas_font.renderText(frame_display + std::to_wstring(frame_current), vec3(0.17f, 0.57f, 0.69f), 6, 6);

		kai_font.renderText(stringToWString("你好，世界！"), vec3(0.17f, 0.57f, 0.69f), 240, 120);
		kai_font.renderText(stringToWString("こんにちは，世界！"), vec3(0.17f, 0.57f, 0.69f), 300, 150);
		kai_font.renderText(stringToWString("Hello, World!"), vec3(0.17f, 0.57f, 0.69f), 360, 180);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}