#include "KHeader.h"
#include "math/vector/vec2.h"
#include "math/vector/vec3.h"
#include "math/vector/vec4.h"
#include "math/function.h"

int main() {
	using namespace KEngines;
	using namespace KEngines::KVector;
	using namespace KEngines::KFunction;

	vec2 v = (vec2(0.f) *= 2);
	std::cout << "Hello KEngines!" << std::endl;
	std::cout << vec2().normalize() << std::endl;
	std::cout << (vec2(2.f) * 3) << std::endl;
	std::cout << (3 * vec2(2.f)) << std::endl;
	std::cout << normalize(vec4(2.f)) << std::endl;
	std::cout << vec3::cross(vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 1.f)) << std::endl;
	std::cin.get();
	return 0;
}