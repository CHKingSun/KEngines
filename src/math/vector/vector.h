//
// Created by KingSun on 2018/06/15
//

#ifndef KENGINES_VECTOR_H
#define KENGINES_VECTOR_H

#include <cassert>
#include <iosfwd>
#include "../../KHeader.h"

namespace KEngines { namespace KVector {

	//define a basic vector template struct
	template <typename T, Kuint N>
	struct basic_vector {
		template <typename T, Kuint N>
		friend std::istream& operator >> (std::istream &is, basic_vector<T, N>& v);
		template <typename T, Kuint N>
		friend std::ostream& operator<<(std::ostream &os, const basic_vector<T, N>& v);

	protected:
		T values[N];

	public:
		basic_vector<T, N>() { set(static_cast<T>(0)); }
		basic_vector<T, N>(const basic_vector<T, N>& v) { this->operator=(v); }

		inline Kuint dimension()const {
			return N;
		}

		inline const T* data()const {
			return values;
		}
		//return reference, for assign
		inline T& operator[](Kuint n) {
			assert(n >= 0 && n < N);
			return this->values[n];
		}
		//for const object, just get value.
		inline const T& operator[](Kuint n)const {
			assert(n >= 0 && n < N);
			return this->values[n];
		}
		virtual inline void set(const T& c) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] = c;
		}

		inline Kboolean operator==(const basic_vector<T, N>& v)const {
			for(Kuint i = 0; i < N; ++i)
				if (this->values[i] != v.values[i])
					return false;
			return true;
		}
		inline Kboolean operator!=(const basic_vector<T, N>& v)const {
			return !this->operator==(v);
		}

		inline basic_vector<T, N>& operator=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] = v.values[i];
			return *this;
		}
		inline basic_vector<T, N>& operator+=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] += v.values[i];
			return *this;
		}
		inline basic_vector<T, N>& operator-=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] -= v.values[i];
			return *this;
		}
		inline basic_vector<T, N>& operator*=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] *= v.values[i];
			return *this;
		}
		inline basic_vector<T, N>& operator/=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] /= v.values[i];
			return *this;
		}

		template <typename C>
		inline basic_vector<T, N>& operator/=(const C& c) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] /= c;
			return *this;
		}
		template <typename C>
		inline basic_vector<T, N>& operator*=(const C& c) {
			for (Kuint i = 0; i < N; ++i)
				this->values[i] /= c;
			return *this;
		}

		template <typename F = Kfloat>
		inline F length()const {
			return sqrt(this->dot<F>(*this));
		}
		template <typename F = Kfloat>
		inline F dot(const basic_vector<T, N>& v)const {
			F ret = 0;
			for (Kuint i = 0; i < N; ++i)
				ret += this->values[i] * v.values[i];
			return ret;
		}
		inline basic_vector<T, N>& normalize() {
			const auto len = length<Kdouble>();
			if (!isZero(len)) this->operator/=(len);
			else this->set(static_cast<T>(KNAN));
			return *this;
		}
	};

	//note: basic_vector is a abstract struct,
	//and can not be initialized directly
	template <typename T, Kuint N>
	const basic_vector<T, N> operator-(const basic_vector<T, N>& v) {
		return basic_vector<T, N>() -= v;
	}
	template <typename T, Kuint N>
	const basic_vector<T, N> operator+(const basic_vector<T, N>& v1,
		const basic_vector<T, N>& v2) {
		return basic_vector<T, N>(v1) += v2;
	}
	template <typename T, Kuint N>
	const basic_vector<T, N> operator-(const basic_vector<T, N>& v1,
		const basic_vector<T, N>& v2) {
		return basic_vector<T, N>(v1) -= v2;
	}
	template <typename T, Kuint N>
	const basic_vector<T, N> operator*(const basic_vector<T, N>& v1,
		const basic_vector<T, N>& v2) {
		return basic_vector<T, N>(v1) *= v2;
	}
	template <typename T, Kuint N>
	const basic_vector<T, N> operator/(const basic_vector<T, N>& v1,
		const basic_vector<T, N>& v2) {
		return basic_vector<T, N>(v1) /= v2;
	}
	template <typename T, Kuint N, typename C>
	const basic_vector<T, N> operator*(const basic_vector<T, N>& v, const C& c) {
		return basic_vector<T, N>(v) *= c;
	};
	template <typename T, Kuint N, typename C>
	const basic_vector<T, N> operator*(const C& c, const basic_vector<T, N>& v) {
		return basic_vector<T, N>(v) *= c;
	};
	template <typename T, Kuint N, typename C>
	const basic_vector<T, N> operator/(const basic_vector<T, N>& v, const C& c) {
		return basic_vector<T, N>(v) /= c;
	};

	template <typename T, Kuint N>
	std::istream& operator>>(std::istream &is, basic_vector<T, N>& v) {
		for (Kuint i = 0; i < N; ++i)
			is >> v.values[i];
		return is;
	}
	template <typename T, Kuint N>
	std::ostream& operator<<(std::ostream &os, const basic_vector<T, N>& v) {
		for (Kuint i = 0; i < N; ++i) {
			os << v.values[i];
			if (i != N - 1) os << " ";
		}
		return os;
	}

} }

#endif // !KENGINES_VECTOR_H

