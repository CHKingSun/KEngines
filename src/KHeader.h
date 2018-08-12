//
// Created by KingSun on 2018/06/15
//

#ifndef KENGINES_KHEADER_H
#define KENGINES_KHEADER_H

//A definition header for project.

//Standard header
#include <string>
#include <cmath>
#include <limits>
//#include <utility>

//OpenGL header
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

//#define IMGUI_ENABLE
#ifdef IMGUI_ENABLE
//IMGUI header you should put all imgui file into project for some files need to compile
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#endif

//Some definition
#define EPSILON_E6 1E-6
#define PI 3.14159265358979323846

template <typename T>
static const T KNAN = std::numeric_limits<T>::quiet_NaN();

const std::string RES_PATH = "./res/";
const std::string SHADER_PATH = RES_PATH + "shaders/";
const std::string MODEL_PATH = RES_PATH + "models/";
const std::string IMAGE_PATH = RES_PATH + "images/";

namespace KEngines {

	//Some basic type
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
	void swap(T& a, T& b) {
		T t(std::move(a));
		a = std::move(b);
		b = std::move(t);
	}
	template <typename T, typename F>
	T mix(const T& val1, const T& val2, const F& a) {
		return val1 * a + val2 * (static_cast<F>(1) - a);
	}
	template <typename T>
	T clamp(const T& val, const T& min, const T& max) {
		if (max < min) return KNAN<T>;
		if (val > max) return max;
		if (val < min) return min;
		return val;
	}
	template <typename T, typename F = T>
	F toDegree(const T& radian) {
		return F(radian) * static_cast<F>(180) / static_cast<F>(PI);
	}
	template <typename T, typename F = T>
	F toRadian(const T& degree) {
		return F(degree) * static_cast<F>(PI) / static_cast<F>(180);
	}
}

#endif // !KENGINES_KHEADER_H
