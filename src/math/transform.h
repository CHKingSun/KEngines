//
// Created by KingSun on 2zeroone8/zero6/29
//

#ifndef KENGINES_TRANSFORM_H
#define KENGINES_TRANSFORM_H

#include "matrix/mat4.h"

namespace KEngines { namespace KFunction {
	using namespace KEngines::KVector;
	using namespace KEngines::KMatrix;

	template <typename T>
	gmat4<T> translate(const gvec3<T>& v) {
		auto zero = static_cast<T>(0);
		auto one = static_cast<T>(1);

		return gmat4<T>(
			one, zero, zero, v.x,
			zero, one, zero, v.y,
			zero, zero, one, v.z,
			zero, zero, zero, one
		);
	}

	template <typename T>
	gmat4<T> rotate(const T& angle, const gvec3<T>& v) {
		const auto ang = toRadian<T>(angle);
		const auto cosA = static_cast<T>(cos(ang));
		const auto sinA = static_cast<T>(sin(ang));

		gvec3<T> axis(normalize(v));
		gvec3<T> tmp((static_cast<T>(1) - cosA) * axis);
		gmat4<T> m;

		m[0][0] = cosA + axis.x * tmp.x;
		m[0][1] = axis.x * tmp.y - axis.z * sinA;
		m[0][2] = axis.x * tmp.z + axis.y * sinA;

		m[1][0] = axis.y * tmp.x + axis.z * sinA;
		m[1][1] = cosA + axis.y * tmp.y;
		m[1][2] = axis.y * tmp.z - axis.x * sinA;

		m[2][0] = axis.z * tmp.x - axis.y * sinA;
		m[2][1] = axis.z * tmp.y + axis.x * sinA;
		m[2][2] = cosA + axis.z * tmp.z;

		return m;
	}

	template <typename T>
	gmat4<T> scale(const gvec3<T>& v) {
		auto zero = static_cast<T>(0);
		auto one = static_cast<T>(1);

		return gmat4<T>(
			v.x, zero, zero, zero,
			zero, v.y, zero, zero,
			zero, zero, v.z, zero,
			zero, zero, zero, one
		);
	}

	template <typename T>
	gmat4<T> lookAt(const gvec3<T>& eye, const gvec3<T>& center, const gvec3<T>& up) {
		//if eye == center or up = 0, the matrix will be nan
		//u-v-n is left-hand coordinate
		const gvec3<T> n(normalize(center - eye));
		const gvec3<T> u(normalize(gvec3<T>::cross(n, up)));
		const gvec3<T> v(normalize(gvec3<T>::cross(u, n)));

		auto zero = static_cast<T>(0);
		auto one = static_cast<T>(1);

		return gmat4<T>(
			 u.x,  u.y,  u.z, -dot(u, eye),
			 v.x,  v.y,  v.z, -dot(v, eye),
			-n.x, -n.y, -n.z,  dot(n, eye),
			zero, zero, zero, one
		);
	}
} }

#endif // !KENGINES_TRANSFORM_H
