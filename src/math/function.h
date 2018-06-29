//
// Created by KingSun on 2018/06/15
//

#ifndef KENGINES_FUNCTION_H
#define KENGINES_FUNCTION_H

#include "../KHeader.h"

namespace KEngines { namespace KFunction {
	//vector functions
	template <typename T, template <typename> typename vec_type, typename F = T>
	F dot(const vec_type<T>& vec1, const vec_type<T>& vec2) {
		return vec1.dot<F>(vec2);
	};

	template <typename T, template <typename> typename vec_type, typename F = T>
	F length(const vec_type<T>& vec) {
		return vec.length<F>();
	};

	template <typename T, template <typename> typename vec_type, typename F = T>
	F distance(const vec_type<T>& vec1, const vec_type<T>& vec2) {
		return length<F, T>(vec1 - vec2);
	};

	template <typename T, template <typename> typename vec_type>
	const vec_type<T> normalize(const vec_type<T>& vec) {
		return vec_type<T>(vec).normalize();
	}

	template <typename T, template <typename> typename vec_type, typename F = T>
	vec_type<T> faceforward(const vec_type<T> &N, const vec_type<T> &I, const vec_type<T> Nref) {
		if (dot<T, vec_type, F>(Nref, I) < static_cast<F>(0)) {
			return N;
		}
		else {
			return -N;
		}
	};

	template <typename T, template <typename> typename vec_type>
	vec_type<T> reflect(const vec_type<T> &I, const vec_type<T> &N) {
		const vec_type<T> n = normalize(N);
		return I - static_cast<T>(2) * dot(n, I) * n;
	};

	template <typename T, template <typename> typename vec_type, typename F = T>
	vec_type<T> refract(const vec_type<T> &I, const vec_type<T> &N, const F &eta) {
		const vec_type<T> i = normalize(I);
		const vec_type<T> n = normalize(N);
		const F cosni = dot<T, vec_type, F>(n, i);

		const F k = static_cast<F>(1) - eta * eta * (static_cast<F>(1) - cosni * cosni);
		if (k < static_cast<F>(0)) {
			return vec_type<T>();
		}
		else {
			return eta * i - (eta * cosni + sqrt(k)) * n;
		}
	};

	//matrix functions
	template <typename T, template<typename> typename mat_type>
	mat_type<T> inverse(const mat_type<T> &m) {
		return mat_type<T>(m).inverse();
	};

	template <typename T, template<typename> typename mat_type>
	mat_type<T> transpose(const mat_type<T> &m) {
		return mat_type<T>(m).transpose();
	};

	template <typename T, template<typename> typename mat_type, typename F = T>
	F determinant(const mat_type<T> &m) {
		return m.determinant();
	};
} }

#endif // !KENGINES_FUNCTION_H
