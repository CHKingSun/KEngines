//
// Created by KingSun on 2018/06/15
//

#ifndef KENGINES_FUNCTION_H
#define KENGINES_FUNCTION_H

#include "../KHeader.h"
#include "vector/vector.h"

namespace KEngines { namespace KFunction {
	using namespace KVector;

	template <typename F, typename T, Kuint N>
	F dot(const basic_vector<T, N>& vec1, const basic_vector<T, N>& vec2) {
		return vec1.dot<F>(vec2);
	};

	template <typename F, typename T, Kuint N>
	F length(const basic_vector<T, N>& vec) {
		return vec.length<F>();
	};

	template <typename F, typename T, Kuint N>
	F distance(const basic_vector<T, N>& vec1, const basic_vector<T, N>& vec2) {
		return length<F, T, N>(vec1 - vec2);
	};

	template <typename T, Kuint N>
	basic_vector<T, N> normalize(basic_vector<T, N> vec) {
		return vec.normalize();
	}


} }

#endif // !KENGINES_FUNCTION_H
