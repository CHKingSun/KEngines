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

} }

#endif // !KENGINES_FUNCTION_H
