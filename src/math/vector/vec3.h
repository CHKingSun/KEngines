//
// Created by KingSun on 2018/06/24
//

#ifndef KENGINES_VEC3_H
#define KENGINES_VEC3_H

#include "vec2.h"

namespace KEngines { namespace KVector {

	template <typename T>
	struct gvec3 {
		union {
			T values[3];
			T x, y, z;
			T r, g, b;
		};

		gvec3(const T& c = static_cast<T>(0)) : x(c), y(c), z(c) {}
		gvec3(const gvec3<T>& v) : x(v.x), y(v.y), z(v.z) {}
		gvec3(const gvec2<T>& v, const T& c) : x(v.x), y(v.y), z(c) {}
		gvec3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}

		inline const T& operator[](Kuint n)const {
			assert(n < 3);
			return this->values[n];
		}
		inline T& operator[](Kuint n) {
			assert(n < 3);
			return this->values[n];
		}
		inline const T& data()const {
			return this->values;
		}

		inline void set(const T& x, const T& y, const T& z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		inline void set(const T& c) {
			this->x = c;
			this->y = c;
			this->z = c;
		}

		inline Kboolean operator==(const gvec3<T>& v)const {
			return this->x = v.x && this->y = v.y && this->z = v.z;
		}
		inline Kboolean operator!=(const gvec3<T>& v)const {
			return !(this->operator==(v));
		}

		inline gvec3<T>& operator=(const gvec3<T>& v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			return *this;
		}
		inline gvec3<T>& operator+=(const gvec3<T>& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			return *this;
		}
		inline gvec3<T>& operator-=(const gvec3<T>& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			return *this;
		}
		inline gvec3<T>& operator*=(const gvec3<T>& v) {
			this->x *= v.x;
			this->y *= v.y;
			this->z *= v.z;
			return *this;
		}
		inline gvec3<T>& operator/=(const gvec3<T>& v) {
			this->x /= v.x;
			this->y /= v.y;
			this->z /= v.z;
			return *this;
		}

	};

} }

#endif // !KENGINES_VEC3_H
