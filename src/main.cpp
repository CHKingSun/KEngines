#include <iostream>

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

int main() {
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
	return 0;
}