//
// Created by KingSun on 2018/06/29
//

#ifndef KENGINES_MAT4_H
#define KENGINES_MAT4_H

#include "mat3.h"
#include "../vector/vec4.h"

namespace KEngines { namespace KMatrix {

	template <typename T>
	struct gmat4 {
		using Row = KVector::gvec4<T>;

		template <typename FT>
		inline friend std::istream& operator>>(std::istream& is, gmat4<FT>& m);
		template <typename FT>
		inline friend std::ostream& operator<<(std::ostream& os, const gmat4<FT>& m);
		template <typename FT>
		friend const KVector::gvec4<FT> operator*(const gmat4<FT>& m, const KVector::gvec4<FT>& v);

	private:
		Row values[4];

	public:
		gmat4(const T& c = static_cast<T>(1)) {
			const auto zero = static_cast<T>(0);

			this->values[0] = Row(c, zero, zero, zero);
			this->values[1] = Row(zero, c, zero, zero);
			this->values[2] = Row(zero, zero, c, zero);
			this->values[3] = Row(zero, zero, zero, c);
		}
		gmat4(const Row& diag) {
			const auto zero = static_cast<T>(0);

			this->values[0] = Row(diag.x, zero, zero, zero);
			this->values[1] = Row(zero, diag.y, zero, zero);
			this->values[2] = Row(zero, zero, diag.z, zero);
			this->values[3] = Row(zero, zero, zero, diag.w);
		}
		gmat4(const gmat4<T>& m) {
			this->values[0] = m.values[0];
			this->values[1] = m.values[1];
			this->values[2] = m.values[2];
			this->values[3] = m.values[3];
		}
		gmat4(const Row& v0, const Row& v1, const Row& v2, const Row& v3) {
			this->values[0] = v0;
			this->values[1] = v1;
			this->values[2] = v2;
			this->values[3] = v3;
		}
		gmat4(const T& x0, const T& y0, const T& z0, const T& w0,
			  const T& x1, const T& y1, const T& z1, const T& w1,
			  const T& x2, const T& y2, const T& z2, const T& w2,
			  const T& x3, const T& y3, const T& z3, const T& w3) {
			this->values[0] = Row(x0, y0, z0, w0);
			this->values[1] = Row(x1, y1, z1, w1);
			this->values[2] = Row(x2, y2, z2, w2);
			this->values[3] = Row(x3, y3, z3, w3);
		}
		gmat4(const gmat3<T>& m) {
			const auto zero = static_cast<T>(0);
			const auto one = static_cast<T>(1);

			this->values[0] = Row(m[0], zero);
			this->values[1] = Row(m[1], zero);
			this->values[2] = Row(m[2], zero);
			this->values[3] = Row(zero, zero, zero, one);
		}

		const T* data()const {
			return &this->values[0][0];
		}
		const Row& operator[](Kuint n)const {
			assert(n < 4);
			return this->values[n];
		}
		Row& operator[](Kuint n) {
			assert(n < 4);
			return this->values[n];
		}

		Kboolean operator==(const gmat4<T>& m)const {
			return this->values[0] == m.values[0]
				&& this->values[1] == m.values[1]
				&& this->values[2] == m.values[2]
				&& this->values[3] == m.values[3];
		}
		Kboolean operator!=(const gmat4<T>& m)const {
			return !(this->operator==(m));
		}

		gmat4<T>& operator=(const gmat4<T>& m) {
			this->values[0] = m.values[0];
			this->values[1] = m.values[1];
			this->values[2] = m.values[2];
			this->values[3] = m.values[3];

			return *this;
		}
		gmat4<T>& operator+=(const gmat4<T>& m) {
			this->values[0] += m.values[0];
			this->values[1] += m.values[1];
			this->values[2] += m.values[2];
			this->values[3] += m.values[3];

			return *this;
		}
		gmat4<T>& operator-=(const gmat4<T>& m) {
			this->values[0] -= m.values[0];
			this->values[1] -= m.values[1];
			this->values[2] -= m.values[2];
			this->values[3] -= m.values[3];

			return *this;
		}
		gmat4<T>& operator*=(const gmat4<T>& m) {
			//matrix multiply. m * n != n * m;
			/**
			* gmat4<T> row(*this);
			* gmat4<T> col(m).transpose();
			* for(int i = 0; i < 4; ++i){
			*      for(int j = 0; j < 4; ++j){
			*          this->values[i][j] = row[i].dot(col[j]);
			*      }
			* }
			*/

			Row row0(this->values[0]);
			Row row1(this->values[1]);
			Row row2(this->values[2]);
			Row row3(this->values[3]);

			Row col0(m.values[0].x, m.values[1].x, m.values[2].x, m.values[3].x);
			Row col1(m.values[0].y, m.values[1].y, m.values[2].y, m.values[3].y);
			Row col2(m.values[0].z, m.values[1].z, m.values[2].z, m.values[3].z);
			Row col3(m.values[0].w, m.values[1].w, m.values[2].w, m.values[3].w);

			this->values[0].x = row0.dot(col0);
			this->values[0].y = row0.dot(col1);
			this->values[0].z = row0.dot(col2);
			this->values[0].w = row0.dot(col3);

			this->values[1].x = row1.dot(col0);
			this->values[1].y = row1.dot(col1);
			this->values[1].z = row1.dot(col2);
			this->values[1].w = row1.dot(col3);

			this->values[2].x = row2.dot(col0);
			this->values[2].y = row2.dot(col1);
			this->values[2].z = row2.dot(col2);
			this->values[2].w = row2.dot(col3);

			this->values[3].x = row3.dot(col0);
			this->values[3].y = row3.dot(col1);
			this->values[3].z = row3.dot(col2);
			this->values[3].w = row3.dot(col3);

			return *this;
		}

		template <typename C>
		gmat4<T>& operator+=(const C& c) {
			this->values[0] += c;
			this->values[1] += c;
			this->values[2] += c;
			this->values[3] += c;

			return *this;
		}
		template <typename C>
		gmat4<T>& operator-=(const C& c) {
			this->values[0] -= c;
			this->values[1] -= c;
			this->values[2] -= c;
			this->values[3] -= c;

			return *this;
		}
		template <typename C>
		gmat4<T>& operator*=(const C& c) {
			this->values[0] *= c;
			this->values[1] *= c;
			this->values[2] *= c;
			this->values[3] *= c;

			return *this;
		}
		template <typename C>
		gmat4<T>& operator/=(const C& c) {
			this->values[0] /= c;
			this->values[1] /= c;
			this->values[2] /= c;
			this->values[3] /= c;

			return *this;
		}

		gmat4<T>& transpose() {
			for (int i = 0; i < 4; ++i) {
				for (int j = i + 1; j < 4; ++j) {
					swap(this->values[i][j], this->values[j][i]);
				}
			}

			return *this;
		}
		gmat4<T>& inverse() {
			// | 00 01 02 03 |
			// | 10 11 12 13 |
			// | 20 21 22 23 |
			// | 30 31 32 33 |
			const T det0 = values[2][0] * values[3][1] - values[3][0] * values[2][1];
			const T det1 = values[2][0] * values[3][2] - values[3][0] * values[2][2];
			const T det2 = values[2][0] * values[3][3] - values[3][0] * values[2][3];
			const T det3 = values[2][1] * values[3][2] - values[3][1] * values[2][2];
			const T det4 = values[2][1] * values[3][3] - values[3][1] * values[2][3];
			const T det5 = values[2][2] * values[3][3] - values[3][2] * values[2][3];

			const T cof00 = values[1][1] * det5 - values[1][2] * det4 + values[1][3] * det3;
			const T cof01 = values[1][0] * det5 - values[1][2] * det2 + values[1][3] * det1;
			const T cof02 = values[1][0] * det4 - values[1][1] * det2 + values[1][3] * det0;
			const T cof03 = values[1][0] * det3 - values[1][1] * det1 + values[1][2] * det0;

			const T detM = values[0][0] * cof00 - values[0][1] * cof01 +
				values[0][2] * cof02 - values[0][3] * cof03;
			if (fabs(detM) <= 1E-6) {
				for (auto &value : values)
					for (int j = 0; j < 4; ++j)
						value[j] = KNAN<T>;
				return *this;
			}

			const T cof10 = values[0][1] * det5 - values[0][2] * det4 + values[0][3] * det3;
			const T cof11 = values[0][0] * det5 - values[0][2] * det2 + values[0][3] * det1;
			const T cof12 = values[0][0] * det4 - values[0][1] * det2 + values[0][3] * det0;
			const T cof13 = values[0][0] * det3 - values[0][1] * det1 + values[0][2] * det0;

			const T det6 = values[0][0] * values[1][1] - values[1][0] * values[0][1];
			const T det7 = values[0][0] * values[1][2] - values[1][0] * values[0][2];
			const T det8 = values[0][0] * values[1][3] - values[1][0] * values[0][3];
			const T det9 = values[0][1] * values[1][2] - values[1][1] * values[0][2];
			const T det10 = values[0][1] * values[1][3] - values[1][1] * values[0][3];
			const T det11 = values[0][2] * values[1][3] - values[1][2] * values[0][3];

			const T cof20 = values[3][1] * det11 - values[3][2] * det10 + values[3][3] * det9;
			const T cof21 = values[3][0] * det11 - values[3][2] * det8 + values[3][3] * det7;
			const T cof22 = values[3][0] * det10 - values[3][1] * det8 + values[3][3] * det6;
			const T cof23 = values[3][0] * det9 - values[3][1] * det7 + values[3][2] * det6;
			const T cof30 = values[2][1] * det11 - values[2][2] * det10 + values[2][3] * det9;
			const T cof31 = values[2][0] * det11 - values[2][2] * det8 + values[2][3] * det7;
			const T cof32 = values[2][0] * det10 - values[2][1] * det8 + values[2][3] * det6;
			const T cof33 = values[2][0] * det9 - values[2][1] * det7 + values[2][2] * det6;

			//remember that adjoint matrix need transpose
			values[0][0] = cof00 / detM;
			values[0][1] = -cof10 / detM;
			values[0][2] = cof20 / detM;
			values[0][3] = -cof30 / detM;

			values[1][0] = -cof01 / detM;
			values[1][1] = cof11 / detM;
			values[1][2] = -cof21 / detM;
			values[1][3] = cof31 / detM;

			values[2][0] = cof02 / detM;
			values[2][1] = -cof12 / detM;
			values[2][2] = cof22 / detM;
			values[2][3] = -cof32 / detM;

			values[3][0] = -cof03 / detM;
			values[3][1] = cof13 / detM;
			values[3][2] = -cof23 / detM;
			values[3][3] = cof33 / detM;

			return *this;
		}
		template <typename F = T>
		F determinant()const {
			const F det0 = values[2][0] * values[3][1] - values[3][0] * values[2][1];
			const F det1 = values[2][0] * values[3][2] - values[3][0] * values[2][2];
			const F det2 = values[2][0] * values[3][3] - values[3][0] * values[2][3];
			const F det3 = values[2][1] * values[3][2] - values[3][1] * values[2][2];
			const F det4 = values[2][1] * values[3][3] - values[3][1] * values[2][3];
			const F det5 = values[2][2] * values[3][3] - values[3][2] * values[2][3];

			const F cof00 = values[1][1] * det5 - values[1][2] * det4 + values[1][3] * det3;
			const F cof01 = values[1][0] * det5 - values[1][2] * det2 + values[1][3] * det1;
			const F cof02 = values[1][0] * det4 - values[1][1] * det2 + values[1][3] * det0;
			const F cof03 = values[1][0] * det3 - values[1][1] * det1 + values[1][2] * det0;

			return  values[0][0] * cof00 - values[0][1] * cof01 +
					values[0][2] * cof02 - values[0][3] * cof03;
		}

		const gmat3<T> toMat3()const {
			return gmat3<T>(
				values[0].x, values[0].y, values[0].z,
				values[1].x, values[1].y, values[1].z,
				values[2].x, values[2].y, values[2].z;
			);
		}
	};

	template <typename T>
	const gmat4<T> operator-(const gmat4<T>& m) {
		return gmat4<T>(0) -= m;
	}
	template <typename T>
	const gmat4<T> operator+(const gmat4<T>& m1, const gmat4<T>& m2) {
		return gmat4<T>(m1) += m2;
	}
	template <typename T>
	const gmat4<T> operator-(const gmat4<T>& m1, const gmat4<T>& m2) {
		return gmat4<T>(m1) -= m2;
	}
	template <typename T>
	const gmat4<T> operator*(const gmat4<T>& m1, const gmat4<T>& m2) {
		return gmat4<T>(m1) *= m2;
	}

	template <typename T, typename C>
	const gmat4<T> operator+(const gmat4<T>& m, const C& c) {
		return gmat4<T>(m) += c;
	}
	template <typename T, typename C>
	const gmat4<T> operator+(const C& c, const gmat4<T>& m) {
		return gmat4<T>(m) += c;
	}
	template <typename T, typename C>
	const gmat4<T> operator-(const gmat4<T>& m, const C& c) {
		return gmat4<T>(m) -= c;
	}
	template <typename T, typename C>
	const gmat4<T> operator-(const C& c, const gmat4<T>& m) {
		return gmat4<T>(m) -= c;
	}
	template <typename T, typename C>
	const gmat4<T> operator*(const gmat4<T>& m, const C& c) {
		return gmat4<T>(m) *= c;
	}
	template <typename T, typename C>
	const gmat4<T> operator*(const C& c, const gmat4<T>& m) {
		return gmat4<T>(m) *= c;
	}
	template <typename T, typename C>
	const gmat4<T> operator/(const gmat4<T>& m, const C& c) {
		return gmat4<T>(m) /= c;
	}

	template <typename T>
	const KVector::gvec4<T> operator*(const gmat4<T>& m, const KVector::gvec4<T>& v) {
		return KVector::gvec4<T>(
			m.values[0].dot(v),
			m.values[1].dot(v),
			m.values[2].dot(v),
			m.values[3].dot(v)
		);
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, gmat4<T>& m) {
		is >> m.values[0] >> m.values[1] >> m.values[2] >> m.values[3];

		return is;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const gmat4<T>& m) {
		os  << m.values[0] << "\n"
			<< m.values[1] << "\n"
			<< m.values[2] << "\n"
			<< m.values[3];

		return os;
	}

	using  mat4 = gmat4<Kfloat>;
	using dmat4 = gmat4<Kdouble>;
	using imat4 = gmat4<Kint>;
	using umat4 = gmat4<Kuint>;
	using bmat4 = gmat4<Kboolean>;
} }

#endif // !KENGINES_MAT4_H

