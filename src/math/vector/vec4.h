//
// Created by KingSun on 2018/06/25
//

#ifndef KENGINES_VEC4_H
#define KENGINES_VEC4_H

#include "vec3.h"

namespace KEngines { namespace KVector {

	template <typename T>
	struct gvec4 {
		union {
			T values[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T s, t, p, q; };
		};

		gvec4<T>(const T& c = static_cast<T>(0)) : x(c), y(c), z(c), w(c) {}
		gvec4<T>(const gvec4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		gvec4<T>(const gvec2<T>& v1, const gvec2<T>& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
		gvec4<T>(const gvec3<T>& v, const T& c) : x(v.x), y(v.y), z(v.z), w(c) {}
		gvec4<T>(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}

		inline const T& operator[](Kuint n)const {
			assert(n < 4);
			return this->values[n];
		}
		inline T& operator[](Kuint n) {
			assert(n < 4);
			return this->values[n];
		}
		inline const T& data()const {
			return this->values;
		}

		inline void set(const T& x, const T& y, const T& z, const T& w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		inline void set(const T& c) {
			this->x = c;
			this->y = c;
			this->z = c;
			this->w = c;
		}

		inline Kboolean operator==(const gvec4<T>& v)const {
			return this->x == v.x && this->y == v.y
				&& this->z == v.z && this->w == w;
		}
		inline Kboolean operator!=(const gvec4<T>& v)const {
			return !(this->operator==(v));
		}

		inline gvec4<T>& operator=(const gvec4<T>& v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = v.w;
			return *this;
		}
		inline gvec4<T>& operator+=(const gvec4<T>& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			this->w += v.w;
			return *this;
		}
		inline gvec4<T>& operator-=(const gvec4<T>& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			this->w -= v.w;
			return *this;
		}
		inline gvec4<T>& operator*=(const gvec4<T>& v) {
			this->x *= v.x;
			this->y *= v.y;
			this->z *= v.z;
			this->w *= v.w;
			return *this;
		}
		inline gvec4<T>& operator/=(const gvec4<T>& v) {
			this->x /= v.x;
			this->y /= v.y;
			this->z /= v.z;
			this->w /= v.w;
			return *this;
		}

		template <typename C>
		inline gvec4<T>& operator+=(const C& c) {
			this->x += c;
			this->y += c;
			this->z += c;
			this->w += c;
			return *this;
		}
		template <typename C>
		inline gvec4<T>& operator-=(const C& c) {
			this->x -= c;
			this->y -= c;
			this->z -= c;
			this->w -= c;
			return *this;
		}
		template <typename C>
		inline gvec4<T>& operator*=(const C& c) {
			this->x *= c;
			this->y *= c;
			this->z *= c;
			this->w *= c;
			return *this;
		}
		template <typename C>
		inline gvec4<T>& operator/=(const C& c) {
			this->x /= c;
			this->y /= c;
			this->z /= c;
			this->w /= c;
			return *this;
		}

		template <typename F = T>
		inline F length()const {
			return static_cast<F>(sqrt(this->dot(*this)));
		}
		template <typename F = T>
		inline F dot(const gvec4<T>& v)const {
			auto ret = static_cast<F>(0);
			ret += this->x * v.x;
			ret += this->y * v.y;
			ret += this->z * v.z;
			ret += this->w * v.w;
			return ret;
		}
		inline gvec4<T>& normalize() {
			const T len = length();
			if (!isZero(len)) this->operator/=(len);
			else this->set(static_cast<T>(KNAN));
			return *this;
		}
	};

	template <typename T>
	const gvec4<T> operator-(const gvec4<T>& v) {
		return gvec4<T>() -= v;
	}
	template <typename T>
	const gvec4<T> operator+(const gvec4<T>& v1, const gvec4<T>& v2) {
		return gvec4<T>(v1) += v2;
	}
	template <typename T>
	const gvec4<T> operator-(const gvec4<T>& v1, const gvec4<T>& v2) {
		return gvec4<T>(v1) -= v2;
	}
	template <typename T>
	const gvec4<T> operator*(const gvec4<T>& v1, const gvec4<T>& v2) {
		return gvec4<T>(v1) *= v2;
	}
	template <typename T>
	const gvec4<T> operator/(const gvec4<T>& v1, const gvec4<T>& v2) {
		return gvec4<T>(v1) /= v2;
	}

	template <typename T, typename C>
	const gvec4<T> operator+(const gvec4<T>&v, const C& c) {
		return gvec4<T>(v) += c;
	}
	template <typename T, typename C>
	const gvec4<T> operator+(const C& c, const gvec4<T>&v) {
		return gvec4<T>(v) += c;
	}
	template <typename T, typename C>
	const gvec4<T> operator-(const gvec4<T>&v, const C& c) {
		return gvec4<T>(v) -= c;
	}
	template <typename T, typename C>
	const gvec4<T> operator-(const C& c, const gvec4<T>&v) {
		return gvec4<T>(v) -= c;
	}
	template <typename T, typename C>
	const gvec4<T> operator*(const gvec4<T>&v, const C& c) {
		return gvec4<T>(v) *= c;
	}
	template <typename T, typename C>
	const gvec4<T> operator*(const C& c, const gvec4<T>&v) {
		return gvec4<T>(v) *= c;
	}
	template <typename T, typename C>
	const gvec4<T> operator/(const gvec4<T>&v, const C& c) {
		return gvec4<T>(v) /= c;
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, gvec4<T>& v) {
		is >> v.x >> v.y >> v.z >> v.w;
		return is;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const gvec4<T>& v) {
		os << v.x << " " << v.y << " " << v.z << " " << v.w;
		return os;
	}

	using vec4 = gvec4<Kfloat>;
	using dvec4 = gvec4<Kdouble>;
	using ivec4 = gvec4<Kint>;
	using uvec4 = gvec4<Kuint>;
	using bvec4 = gvec4<Kboolean>;
} }

#endif // !KENGINES_VEC4_H
