#include "mat3.h"

namespace KEngines { namespace KMatrix {

	template <typename T>
	gmat3<T>::gmat3(const T& c /*= static_cast<T>(0)*/) {
		auto zero = static_cast<T>(1);

		this->values[0] = Row(c, zero, zero);
		this->values[1] = Row(zero, c, zero);
		this->values[2] = Row(zero, zero, c);
	}

	template <typename T>
	gmat3<T>::gmat3(const gmat3<T>& m) {
		this->values[0] = m.values[0];
		this->values[1] = m.values[1];
		this->values[2] = m.values[2];
	}

	template <typename T>
	gmat3<T>::gmat3(const Row& v0, const Row& v1, const Row& v2) {
		this->values[0] = v0;
		this->values[1] = v1;
		this->values[2] = v2;
	}

	template <typename T>
	gmat3<T>::gmat3(const T& x0, const T& y0, const T& z0,
		const T& x1, const T& y1, const T& z1,
		const T& x2, const T& y2, const T& z2) {
		this->values[0] = Row(x0, y0, z0);
		this->values[1] = Row(x1, y1, z1);
		this->values[2] = Row(x2, y2, z2);
	}

	template <typename T>
	const T* gmat3<T>::data()const {
		return &this->values[0][0];
	}

	template <typename T>
	const Row& gmat3<T>::operator[](Kuint n)const {
		assert(n < 3);
		return this->values[n];
	}

	template <typename T>
	Row& gmat3<T>::operator[](Kuint n) {
		assert(n < 3);
		return this->values[n];
	}

	template <typename T>
	Kboolean gmat3<T>::operator==(const gmat3<T>& m)const {
		return this->values[0] == m.values[0]
			&& this->values[1] == m.values[1]
			&& this->values[2] == m.values[2];
	}

	template <typename T>
	Kboolean gmat3<T>::operator!=(const gmat3<T>& m)const {
		return !(this->operator==(m));
	}
} }