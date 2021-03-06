//
// Created by KingSun on 2018/06/15
//

#ifndef KENGINES_VEC2_H
#define KENGINES_VEC2_H

#include "../../KHeader.h"
#include <cassert>
#include <iosfwd>

namespace KEngines { namespace KVector{
	template <typename T>
	struct gvec2 {
		union {
			T values[2];
			struct { T x, y; };
			struct { T s, t; };
		};

		explicit gvec2(const T& c = static_cast<T>(0)) : x(c), y(c) {}
		gvec2(const gvec2<T>& v) : x(v.x), y(v.y) {}
		gvec2(const T& x, const T& y) : x(x), y(y) {}

		const T& operator[](Kuint n)const {
			assert(n < 2);
			return this->values[n];
		}
		T& operator[](Kuint n) {
			assert(n < 2);
			return this->values[n];
		}
		const T* data()const {
			return this->values;
		 }

		void set(const T& x, const T& y) {
			this->x = x;
			this->y = y;
		}
		void set(const T& c) {
			this->x = c;
			this->y = c;
		}

		Kboolean operator==(const gvec2<T>& v)const {
			return this->x == v.x && this->y == v.y;
		}
		Kboolean operator!=(const gvec2<T>& v)const {
			return !(this->operator==(v));
		}

		gvec2<T>& operator=(const gvec2<T>& v) {
			this->x = v.x;
			this->y = v.y;
			return *this;
		}
		gvec2<T>& operator+=(const gvec2<T>& v) {
			this->x += v.x;
			this->y += v.y;
			return *this;
		}
		gvec2<T>& operator-=(const gvec2<T>& v) {
			this->x -= v.x;
			this->y -= v.y;
			return *this;
		}
		gvec2<T>& operator*=(const gvec2<T>& v) {
			this->x *= v.x;
			this->y *= v.y;
			return *this;
		}
		gvec2<T>& operator/=(const gvec2<T>& v) {
			this->x /= v.x;
			this->y /= v.y;
			return *this;
		}

		template <typename C>
		gvec2<T>& operator+=(const C& c) {
			this->x += c;
			this->y += c;
			return *this;
		}
		template <typename C>
		gvec2<T>& operator-=(const C& c) {
			this->x -= c;
			this->y -= c;
			return *this;
		}
		template <typename C>
		gvec2<T>& operator*=(const C& c) {
			this->x *= c;
			this->y *= c;
			return *this;
		}
		template <typename C>
		gvec2<T>& operator/=(const C& c) {
			this->x /= c;
			this->y /= c;
			return *this;
		}

		template <typename F = T>
		F length()const {
			return static_cast<F>(sqrt(this->dot(*this)));
		}
		template <typename F = T>
		F dot(const gvec2<T>& v)const {
			auto ret = static_cast<F>(0);
			ret += this->x * v.x;
			ret += this->y * v.y;
			return ret;
		}
		gvec2<T>& normalize() {
			const T len = length();
			if (!isZero(len)) this->operator/=(len);
			else this->set(KNAN<T>);
			return *this;
		}
	};

	template <typename T>
	const gvec2<T> operator-(const gvec2<T>& v) {
		return gvec2<T>() -= v;
	}
	template <typename T>
	const gvec2<T> operator+(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) += v2;
	}
	template <typename T>
	const gvec2<T> operator-(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) -= v2;
	}
	template <typename T>
	const gvec2<T> operator*(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) *= v2;
	}
	template <typename T>
	const gvec2<T> operator/(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) /= v2;
	}

	template <typename T, typename C>
	const gvec2<T> operator+(const gvec2<T>&v, const C& c) {
		return gvec2<T>(v) += c;
	}
	template <typename T, typename C>
	const gvec2<T> operator+(const C& c, const gvec2<T>&v) {
		return gvec2<T>(v) += c;
	}
	template <typename T, typename C>
	const gvec2<T> operator-(const gvec2<T>&v, const C& c) {
		return gvec2<T>(v) -= c;
	}
	template <typename T, typename C>
	const gvec2<T> operator-(const C& c, const gvec2<T>&v) {
		return gvec2<T>(v) -= c;
	}
	template <typename T, typename C>
	const gvec2<T> operator*(const gvec2<T>&v, const C& c) {
		return gvec2<T>(v) *= c;
	}
	template <typename T, typename C>
	const gvec2<T> operator*(const C& c, const gvec2<T>&v) {
		return gvec2<T>(v) *= c;
	}
	template <typename T, typename C>
	const gvec2<T> operator/(const gvec2<T>&v, const C& c) {
		return gvec2<T>(v) /= c;
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, gvec2<T>& v) {
		is >> v.x >> v.y;
		return is;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const gvec2<T>& v) {
		os << v.x << " " << v.y;
		return os;
	}

	using  vec2 = gvec2<Kfloat>;
	using dvec2 = gvec2<Kdouble>;
	using ivec2 = gvec2<Kint>;
	using uvec2 = gvec2<Kuint>;
	using bvec2 = gvec2<Kboolean>;
} }

#endif // !KENGINES_VEC2_H

