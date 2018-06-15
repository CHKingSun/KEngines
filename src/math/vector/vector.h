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
	protected:
		template <typename T, Kuint N>
		friend std::istream& operator >> (std::istream &is, basic_vector<T, N>& v);
		template <typename T, Kuint N>
		friend std::ostream& operator<<(std::ostream &os, const basic_vector<T, N>& v);

		virtual inline T& at(Kuint n) = 0;
		virtual inline const T& at(Kuint n)const = 0;
		basic_vector<T, N>() = default;

	public:

		inline Kuint dimension()const {
			return N;
		}

		virtual inline const T* data()const = 0;
		//return reference, for assign
		virtual inline T& operator[](Kuint n) = 0;
		//for const object, just get value.
		virtual inline const T& operator[](Kuint n)const = 0;
		virtual inline void set(const T& c) = 0;

		inline Kboolean operator==(const basic_vector<T, N>& v)const {
			for(Kuint i = 0; i < N; ++i)
				if (this->at(i) != v.at(i))
					return false;
			return true;
		}
		inline Kboolean operator!=(const basic_vector<T, N>& v)const {
			return !this->operator==(v);
		}

		inline basic_vector<T, N>& operator=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->at(i) = v.at(i);
			return *this;
		}
		inline basic_vector<T, N>& operator+=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->at(i) += v.at(i);
			return *this;
		}
		inline basic_vector<T, N>& operator-=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->at(i) -= v.at(i);
			return *this;
		}
		inline basic_vector<T, N>& operator*=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->at(i) *= v.at(i);
			return *this;
		}
		inline basic_vector<T, N>& operator/=(const basic_vector<T, N>& v) {
			for (Kuint i = 0; i < N; ++i)
				this->at(i) /= v.at(i);
			return *this;
		}

		template <typename C>
		inline basic_vector<T, N>& operator/=(const C& c) {
			for (Kuint i = 0; i < N; ++i)
				this->at(i) /= c;
			return *this;
		}
		template <typename C>
		inline basic_vector<T, N>& operator*=(const C& c) {
			for (Kuint i = 0; i < N; ++i)
				this->at(i) /= c;
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
				ret += this->at(i) * v.at(i);
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
	//template <typename T, Kuint N>
	//basic_vector<T, N> operator-(const basic_vector<T, N>& v) {
	//	return basic_vector<T, N>() -= v;
	//}
	//template <typename T, Kuint N>
	//basic_vector<T, N> operator+(const basic_vector<T, N>& v1,
	//	const basic_vector<T, N>& v2) {
	//	return basic_vector<T, N>(v1) += v2;
	//}
	//template <typename T, Kuint N>
	//basic_vector<T, N> operator-(const basic_vector<T, N>& v1,
	//	const basic_vector<T, N>& v2) {
	//	return basic_vector<T, N>(v1) -= v2;
	//}
	//template <typename T, Kuint N>
	//basic_vector<T, N> operator*(const basic_vector<T, N>& v1,
	//	const basic_vector<T, N>& v2) {
	//	return basic_vector<T, N>(v1) *= v2;
	//}
	//template <typename T, Kuint N>
	//basic_vector<T, N> operator/(const basic_vector<T, N>& v1,
	//	const basic_vector<T, N>& v2) {
	//	return basic_vector<T, N>(v1) /= v2;
	//}
	//template <typename T, Kuint N, typename C>
	//basic_vector<T, N> operator*(const basic_vector<T, N>& v, const C& c) {
	//	return basic_vector<T, N>(v) *= c;
	//};
	//template <typename T, Kuint N, typename C>
	//basic_vector<T, N> operator*(const C& c, const basic_vector<T, N>& v) {
	//	return basic_vector<T, N>(v) *= c;
	//};
	//template <typename T, Kuint N, typename C>
	//basic_vector<T, N> operator/(const basic_vector<T, N>& v, const C& c) {
	//	return basic_vector<T, N>(v) /= c;
	//};

	template <typename T, Kuint N>
	std::istream& operator>>(std::istream &is, basic_vector<T, N>& v) {
		for (Kuint i = 0; i < N; ++i)
			is >> v.at(i);
		return is;
	}
	template <typename T, Kuint N>
	std::ostream& operator<<(std::ostream &os, const basic_vector<T, N>& v) {
		for (Kuint i = 0; i < N; ++i) {
			os << v.at(i);
			if (i != N - 1) os << " ";
		}
		return os;
	}

} }

#endif // !KENGINES_VECTOR_H

