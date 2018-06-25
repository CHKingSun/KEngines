//
// Created by KingSun on 2018/06/15
//

#ifndef KENGINES_KHEADER_H
#define KENGINES_KHEADER_H

//A definition header for project.

//Standard header
#include <iostream>
#include <string>
#include <cmath>

//OpenGL header
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define IMGUI_ENABLE
#ifdef IMGUI_ENABLE
//IMGUI header you should put all imgui file into project for some files need to compile
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#endif

//Some definition
#define EPSILON_E6 1E-6
#define PI 3.1415926535
#ifdef KNAN
#undef KNAN
#endif
#define KNAN nan("Nan")

const std::string RES_PATH = "./res/";

//debug
#include <iomanip>
#define glCall(x)	x; \
					glCheckError(#x, __FILE__, __LINE__);
void glClearError() { while (glGetError() != GL_NO_ERROR); } //抛弃当前所有错误;
bool glCheckError(const char* fun, const char* file, int line) {
	GLenum error;
	if ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error at: " << fun << " in file: " << file << " at line: " << line
			<< " with error code: 0x0" << std::setbase(16) << error << std::endl;
		return false;
	}
	return true;
}

namespace KEngines {

	//some basic type
	using Kint = int;
	using Kuint = unsigned int;
	using Kfloat = float;
	using Kdouble = double;
	using Klong = long;
	using Kulong = unsigned long;
	using Kshort = short;
	using Kushort = unsigned short;
	using Kboolean = bool;
	using Ksize = unsigned int;
	using Kchar = char;
	using Kuchar = unsigned char;
	using Kbyte = char;
	using Kubyte = unsigned char;

	//Fundamental functions
	template <typename T>
	T abs(const T& c) {
		return c >= static_cast<T>(0) ? c : -c;
	}
	template <typename T>
	Kboolean isZero(const T& c) {
		return abs(c) <= static_cast<T>(EPSILON_E6);
	}
	template <typename T>
	Kboolean isNan(const T& c) {
		return isnan(c) || isinf(c);
	}
	template <typename T>
	const T& min(const T& a, const T& b) {
		return a < b ? a : b;
	}
	template <typename T>
	const T& max(const T& a, const T& b) {
		return a > b ? a : b;
	}
	template <typename T>
	T clamp(const T& val, const T& min, const T& max) {
		if (max < min) return static_cast<T>(KNAN);
		if (val > max) return max;
		if (val < min) return min;
		return val;
	}
	template <typename T, typename F = Kfloat>
	F toDegree(const T& radian) {
		return F(radian) * static_cast<F>(180) / static_cast<F>(PI);
	}
	template <typename T, typename F = Kfloat>
	F toRadian(const T& degree) {
		return F(degree) * static_cast<F>(PI) / static_cast<F>(180);
	}
}

#endif // !KENGINES_KHEADER_H
