#include "KHeader.h"
#include "math/vector/vec2.h"
#include "math/function.h"

int main() {
	using namespace KEngines;
	using namespace KEngines::KVector;
	using namespace KEngines::KFunction;

	vec2 v = (vec2(0.f) *= 2);
	std::cout << "Hello KEngines!" << std::endl;
	std::cout << vec2().normalize() << std::endl;
	std::cout << (vec2(2.f) * 3.f) << std::endl;
	std::cin.get();
	return 0;
}