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
			struct { T x, y, z; };
			struct { T r, g, b; };
		};

		explicit gvec3(const T& c = static_cast<T>(0)) : x(c), y(c), z(c) {}
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
			return this->x == v.x && this->y == v.y && this->z == v.z;
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

		template <typename C>
		inline gvec3<T>& operator+=(const C& c) {
			this->x += c;
			this->y += c;
			this->z += c;
			return *this;
		}
		template <typename C>
		inline gvec3<T>& operator-=(const C& c) {
			this->x -= c;
			this->y -= c;
			this->z -= c;
			return *this;
		}
		template <typename C>
		inline gvec3<T>& operator*=(const C& c) {
			this->x *= c;
			this->y *= c;
			this->z *= c;
			return *this;
		}
		template <typename C>
		inline gvec3<T>& operator/=(const C& c) {
			this->x /= c;
			this->y /= c;
			this->z /= c;
			return *this;
		}

		template <typename F = T>
		inline F length()const {
			return static_cast<F>(sqrt(this->dot(*this)));
		}
		template <typename F = T>
		inline F dot(const gvec3<T>& v)const {
			auto ret = static_cast<F>(0);
			ret += this->x * v.x;
			ret += this->y * v.y;
			ret += this->z * v.z;
			return ret;
		}
		inline gvec3<T>& normalize() {
			const T len = length();
			if (!isZero(len)) this->operator/=(len);
			else this->set(static_cast<T>(KNAN));
			return *this;
		}

		//The function will change the values.
		//If you do not want to change the values,
		//please use the static one.
		inline gvec3<T>& cross(const gvec3<T>& v) {
			Kfloat tx = this->x, ty = this->y, tz = this->z;
			this->x = ty * v.z - v.y * tz;
			this->y = -(tx * v.z - v.x * tz);
			this->z = tx * v.y - v.x * ty;
			return *this;
		}
		//Note: you will just get the angle between 0 and 180.
		//It means that the angle has no direction.
		template <typename F = T>
		inline F getAngle(const gvec3<T>& v)const {
			auto len1 = this->length<F>();
			auto len2 = v.length<F>();

			if (isZero(len1) || isZero(len2)) return static_cast<F>(KNAN);

			return toDegree<F>(acos(dot<F>(v) / (len1 * len2)));
		}

		static const gvec3<T> cross(const gvec3<T>& v1, const gvec3<T>& v2) {
			return gvec3<T>(v1).cross(v2);
		}
	};

	template <typename T>
	const gvec3<T> operator-(const gvec3<T>& v) {
		return gvec3<T>() -= v;
	}
	template <typename T>
	const gvec3<T> operator+(const gvec3<T>& v1, const gvec3<T>& v2) {
		return gvec3<T>(v1) += v2;
	}
	template <typename T>
	const gvec3<T> operator-(const gvec3<T>& v1, const gvec3<T>& v2) {
		return gvec3<T>(v1) -= v2;
	}
	template <typename T>
	const gvec3<T> operator*(const gvec3<T>& v1, const gvec3<T>& v2) {
		return gvec3<T>(v1) *= v2;
	}
	template <typename T>
	const gvec3<T> operator/(const gvec3<T>& v1, const gvec3<T>& v2) {
		return gvec3<T>(v1) /= v2;
	}

	template <typename T, typename C>
	const gvec3<T> operator+(const gvec3<T>&v, const C& c) {
		return gvec3<T>(v) += c;
	}
	template <typename T, typename C>
	const gvec3<T> operator+(const C& c, const gvec3<T>&v) {
		return gvec3<T>(v) += c;
	}
	template <typename T, typename C>
	const gvec3<T> operator-(const gvec3<T>&v, const C& c) {
		return gvec3<T>(v) -= c;
	}
	template <typename T, typename C>
	const gvec3<T> operator-(const C& c, const gvec3<T>&v) {
		return gvec3<T>(v) -= c;
	}
	template <typename T, typename C>
	const gvec3<T> operator*(const gvec3<T>&v, const C& c) {
		return gvec3<T>(v) *= c;
	}
	template <typename T, typename C>
	const gvec3<T> operator*(const C& c, const gvec3<T>&v) {
		return gvec3<T>(v) *= c;
	}
	template <typename T, typename C>
	const gvec3<T> operator/(const gvec3<T>&v, const C& c) {
		return gvec3<T>(v) /= c;
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, gvec3<T>& v) {
		is >> v.x >> v.y >> v.z;
		return is;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const gvec3<T>& v) {
		os << v.x << " " << v.y << " " << v.z;
		return os;
	}

	using vec3 = gvec3<Kfloat>;
	using dvec3 = gvec3<Kdouble>;
	using ivec3 = gvec3<Kint>;
	using uvec3 = gvec3<Kuint>;
	using bvec3 = gvec3<Kboolean>;
} }

#endif // !KENGINES_VEC3_H
