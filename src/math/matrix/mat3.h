//
// Created by KingSun on 2018/06/27
//

#ifndef KENGINES_MAT3_H
#define KENGINES_MAT3_H

#include "../vector/vec3.h"

namespace KEngines { namespace KMatrix {

	template <typename T>
	struct gmat3 {
		using Row = KVector::gvec3<T>;

	private:
		Row values[3]; //row-first

	public: 
		inline explicit gmat3(const T& c = static_cast<T>(0));
		inline gmat3(const gmat3<T>& m);
		inline gmat3(const Row& v0, const Row& v1, const Row& v2);
		inline gmat3(const T& x0, const T& y0, const T& z0,
						const T& x1, const T& y1, const T& z1,
						const T& x2, const T& y2, const T& z2);

		inline const T *data()const;
		inline const Row& operator[](Kuint n)const;
		inline Row& operator[](Kuint n);

		inline Kboolean operator==(const gmat3<T>& m)const;
		inline Kboolean operator!=(const gmat3<T>& m)const;

		inline gmat3<T>& operator=(const gmat3<T>& m);
		inline gmat3<T>& operator+=(const gmat3<T>& m);
		inline gmat3<T>& operator-=(const gmat3<T>& m);
		inline gmat3<T>& operator*=(const gmat3<T>& m);
		inline gmat3<T>& operator/=(const gmat3<T>& m);
	};

} }

#endif // !KENGINES_MAT3_H

