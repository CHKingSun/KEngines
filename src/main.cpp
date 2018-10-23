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

	quaternion q(60.f, vec3(0.f, 1.f, 0.f));
	Log::debug(q);
	vec3 v1(1.f, 0.f, 1.f);
	vec3 v2 = q * v1;
	Log::debug(v2);
	Log::debug(q.fromVectors(v1, v2));

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
#include "object/Box.h"
#include "object/Group.h"
#include "object/Sphere.h"
#include "util/StringUtil.h"
#include "buffer/FrameBuffer.h"
#include "object/Model.h"
#include "camera/Camera.h"
#include "render/ViewRenderer.h"
#include "render/FirstViewRenderer.h"
#include "render/MazeRenderer.h"
#include "material/Material.h"
#include "material/Texture.h"

using namespace KEngines;
using namespace KEngines::KFunction;
using namespace KEngines::KVector;
using namespace KEngines::KMatrix;
using namespace KEngines::KBuffer;
using namespace KEngines::KRenderer;
using namespace KEngines::KObject;
using namespace KEngines::KUtil;
using namespace KEngines::KCamera;
using namespace KEngines::KMaterial;

void initLocale() {
	Log::info("Local locale: ", std::locale().name().c_str());

	std::setlocale(LC_CTYPE, "");
}

int main() {

#if 0
	//initLocale();

	auto renderer = new FirstViewRenderer("KEngines");

	auto material = new Material(GREY, GREY, GREY, 20.f);
	material->addTexture(new Texture(IMAGE_PATH + "stone.png", AMBIENT));
	material->addTexture(new Texture(IMAGE_PATH + "stone.png", DIFFUSE));
	auto plane = new Plane(30.f, 30.f, 12, 12);
	plane->rotate(quaternion(90.f, vec3(-1.f, 0.f, 0.f)));
	plane->setMaterial(material);

	//auto model = new Model(MODEL_PATH + "SK_Mannequin.FBX");
	//model->rotate(quaternion(-90.f, vec3(1.f, 0.f, 0.f)));
	auto model = new Model(MODEL_PATH + "Alonne/Alonne.obj");
	//auto model = new Model(MODEL_PATH + "cellrain.obj");
	//auto model = new Model(MODEL_PATH + "pokeball.obj");
	model->scale(vec3(3.6f));

	renderer->addObject(plane);
	renderer->addObject(model);
#else

	auto renderer = new MazeRenderer("Maze");

#endif

	renderer->exec();

	delete renderer;

	return 0;
}