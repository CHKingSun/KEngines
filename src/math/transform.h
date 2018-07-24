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
		const auto zero = static_cast<T>(0);
		const auto one = static_cast<T>(1);

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
		return gmat4<T>(gvec4<T>(v, static_cast<T>(1)));
	}

	template <typename T>
	gmat4<T> lookAt(const gvec3<T>& eye, const gvec3<T>& center, const gvec3<T>& up) {
		//if eye == center or up = 0, the matrix will be nan
		//u-v-n is left-hand coordinate
		const gvec3<T> n(normalize(center - eye));
		const gvec3<T> u(normalize(gvec3<T>::cross(n, up)));
		const gvec3<T> v(normalize(gvec3<T>::cross(u, n)));

		const auto zero = static_cast<T>(0);
		const auto one = static_cast<T>(1);

		return gmat4<T>(
			 u.x,  u.y,  u.z, -dot(u, eye),
			 v.x,  v.y,  v.z, -dot(v, eye),
			-n.x, -n.y, -n.z,  dot(n, eye),
			zero, zero, zero, one
		);
	}

	template <typename T>
	gmat4<T> ortho(const T& left, const T& right, const T& bottom,
		const T& top, const T& zNear, const T& zFar) {
		gmat4<T> m;

		m[0][0] = static_cast<T>(2) / (right - left);
		m[1][1] = static_cast<T>(2) / (top - bottom);
		m[2][2] = static_cast<T>(-2) / (zFar - zNear);

		m[0][3] = -(right + left) / (right - left);
		m[1][3] = -(top + bottom) / (top - bottom);
		m[2][3] = -(zFar + zNear) / (zFar - zNear);

		return m;
	}

	template <typename T>
	gmat4<T> frustum(const T& left, const T& right, const T& bottom,
		const T& top, const T& zNear, const T& zFar) {
		gmat4<T> m(0);

		m[0][0] = static_cast<T>(2) * zNear / (right - left);
		m[1][1] = static_cast<T>(2) * zNear / (top - bottom);
		m[0][2] = -(right + left) / (right - left);
		m[1][2] = -(top + bottom) / (top - bottom);

		m[2][2] = -(zFar + zNear) / (zFar - zNear);
		m[2][3] = static_cast<T>(-2) * zFar * zNear / (zFar - zNear);
		m[3][2] = static_cast<T>(-1);

		return m;
	}

	template <typename T>
	gmat4<T> perspective(const T& fovy, const T& aspect, const T& zNear, const T& zFar) {
		if ((aspect <= static_cast<T>(EPSILON_E6)) &&
			zFar <= zNear && fovy <= 0 && fovy > 180.0)
			return gmat4<T>(KNAN<T>);

		const auto cot = static_cast<T>(1.0 / tan(toRadian<Kdouble>(fovy / 2.0)));
		gmat4<T> m(0);

		m[0][0] = cot / aspect;
		m[1][1] = cot;
		m[2][2] = -(zFar + zNear) / (zFar - zNear);
		m[2][3] = static_cast<T>(-2) * zFar * zNear / (zFar - zNear);
		m[3][2] = static_cast<T>(-1);

		return m;
	}

	template <typename T, typename I>
	gvec3<T> project(const gvec3<T>& v, const gmat4<T>& model,
		const gmat4<T>& proj, const gvec4<I>& viewport) {
		gvec4<T> pos(proj * (model * gvec4<T>(v, static_cast<T>(1))));
		//to clipping coordinates
		pos /= pos.w; //to [-1, 1]
		pos /= static_cast<T>(2); //to [-0.5, 0.5]
		pos += static_cast<T>(0.5); //to [0, 1]

		pos.x = pos.x * static_cast<T>(viewport[2])
				+ static_cast<T>(viewport[0]); //to [v[0], v[2]]
		pos.y = pos.y * static_cast<T>(viewport[3])
				+ static_cast<T>(viewport[1]); //to [v[1], v[3]]

		return gvec3<T>(pos.x, pos.y, pos.z);
	}

	template <typename T, typename I>
	gvec3<T> unProject(const gvec3<T>& v, const gmat4<T>& model,
		const gmat4<T>& proj, const gvec4<I>& viewport) {
		gvec4<T> pos(v, static_cast<T>(1));
		pos.x = (pos.x - static_cast<T>(viewport[0]))
				/ static_cast<T>(viewport[2]); //to [0, 1]
		pos.y = (pos.y - static_cast<T>(viewport[1]))
				/ static_cast<T>(viewport[3]); //to [0, 1]
		pos *= static_cast<T>(2); //to [0, 2]
		pos -= static_cast<T>(1); //to [-1, 1]

		gmat4<T> tmp(proj * model);
		pos = tmp.inverse() * pos;
		pos /= pos.w;

		return gvec3<T>(pos.x, pos.y, pos.z);
	}
} }

#endif // !KENGINES_TRANSFORM_H
