//
// Created by KingSun on 2018/08/11
//

#ifndef KENGINES_COLOR_H
#define KENGINES_COLOR_H

#include "vec3.h"

namespace KEngines { namespace KVector {
	using Color = vec3;

	static const Color BLACK(0.f);
	static const Color RED(1.f, 0.f, 0.f);
	static const Color BLUE(0.f, 1.f, 0.f);
	static const Color GREEN(0.f, 0.f, 1.0f);
	static const Color ORANGE(1.f, 1.f, 0.f);
	static const Color YELLOW(0.f, 1.f, 1.f);
	static const Color PINK(1.f, 0.f, 1.f);
	static const Color WHITE(1.f);
	static const Color GREY(0.8f);
	static const Color DARKGREY(0.27f);

} }

#endif // !KENGINES_COLOR_H

