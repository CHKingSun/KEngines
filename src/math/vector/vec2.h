//
// Created by KingSun on 2018/06/15
//

#ifndef KEGINES_VEC2_H
#define KEGINES_VEC2_H

#include "vector.h"

namespace KEngines { namespace KVector{
	template <typename T>
	struct gvec2 : public basic_vector<T, 2> {
	protected:
		inline T& at(Kuint n)override {
			return this->values[n];
		}
		inline const T& at(Kuint n)const override {
			return this->values[n];
		}

	public:
		union {
			T values[2];
			struct { T x, y; };
			struct { T s, t; };
		};

		inline const T* data()const override {
			return this->values;
		}
		inline T& operator[](Kuint n)override {
			assert(n >= 0 && n < 2);
			return this->values[n];
		}
		inline const T& operator[](Kuint n)const override {
			assert(n >= 0 && n < 2);
			return this->values[n];
		}

		gvec2<T>(): gvec2<T>(0) {}
		gvec2<T>(const gvec2<T>& v): x(v.x), y(v.y) {}
		gvec2<T>(const basic_vector<T, 2>& v): x(v.at(0)), y(v.at(1)) {}
		explicit gvec2<T>(const T& c): x(c), y(c) {}
		gvec2<T>(const T& x, const T& y): x(x), y(y) {}

		void set(const T& c)override {
			this->x = c;
			this->y = c;
		}
		void set(const T& x, const T& y) {
			this->x = x;
			this->y = y;
		}
	};

	template <typename T>
	gvec2<T> operator-(const gvec2<T>& v) {
		return gvec2<T>() -= v;
	}
	template <typename T>
	gvec2<T> operator+(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) += v2;
	}
	template <typename T>
	gvec2<T> operator-(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) -= v2;
	}
	template <typename T>
	gvec2<T> operator*(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) *= v2;
	}
	template <typename T>
	gvec2<T> operator/(const gvec2<T>& v1, const gvec2<T>& v2) {
		return gvec2<T>(v1) /= v2;
	}

	template <typename T, typename C>
	gvec2<T> operator*(const gvec2<T>& v, const C& c) {
		return gvec2<T>(v) *= c;
	};
	template <typename T, typename C>
	gvec2<T> operator*(const C& c, const gvec2<T>& v) {
		return gvec2<T>(v) *= c;
	};
	template <typename T, typename C>
	gvec2<T> operator/(const gvec2<T>& v, const C& c) {
		return gvec2<T>(v) /= c;
	};

	using vec2 = gvec2<Kfloat>;
	using dvec2 = gvec2<Kdouble>;
	using ivec2 = gvec2<Kint>;
	using uvec2 = gvec2<Kuint>;
	using bvec2 = gvec2<Kboolean>;
} }

#endif // !KEGINES_VEC2_H

