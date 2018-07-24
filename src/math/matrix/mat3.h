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

		template <typename FT>
		inline friend std::istream& operator>>(std::istream& is, gmat3<FT>& m);
		template <typename FT>
		inline friend std::ostream& operator<<(std::ostream& os, const gmat3<FT>& m);
		template <typename FT>
		friend const KVector::gvec3<FT> operator*(const gmat3<FT>& m, const KVector::gvec3<FT>& v);

	private:
		Row values[3]; //row-first

	public: 
		gmat3(const T& c = static_cast<T>(1)) {
			const auto zero = static_cast<T>(0);

			this->values[0] = Row(c, zero, zero);
			this->values[1] = Row(zero, c, zero);
			this->values[2] = Row(zero, zero, c);
		}
		gmat3(const Row& diag) {
			const auto zero = static_cast<T>(0);

			this->values[0] = Row(diag.x, zero, zero);
			this->values[1] = Row(zero, diag.y, zero);
			this->values[2] = Row(zero, zero, diag.z);
		}
		gmat3(const gmat3<T>& m) {
			this->values[0] = m.values[0];
			this->values[1] = m.values[1];
			this->values[2] = m.values[2];
		}
		gmat3(const Row& v0, const Row& v1, const Row& v2) {
			this->values[0] = v0;
			this->values[1] = v1;
			this->values[2] = v2;
		}
		gmat3(const T& x0, const T& y0, const T& z0,
			  const T& x1, const T& y1, const T& z1,
			  const T& x2, const T& y2, const T& z2) {
			this->values[0] = Row(x0, y0, z0);
			this->values[1] = Row(x1, y1, z1);
			this->values[2] = Row(x2, y2, z2);
		}

		const T* data()const {
			return &this->values[0][0];
		}
		const Row& operator[](Kuint n)const {
			assert(n < 3);
			return this->values[n];
		}
		Row& operator[](Kuint n) {
			assert(n < 3);
			return this->values[n];
		}

		Kboolean operator==(const gmat3<T>& m)const {
			return this->values[0] == m.values[0]
				&& this->values[1] == m.values[1]
				&& this->values[2] == m.values[2];
		}
		Kboolean operator!=(const gmat3<T>& m)const {
			return !(this->operator==(m));
		}

		gmat3<T>& operator=(const gmat3<T>& m) {
			this->values[0] = m.values[0];
			this->values[1] = m.values[1];
			this->values[2] = m.values[2];

			return *this;
		}
		gmat3<T>& operator+=(const gmat3<T>& m) {
			this->values[0] += m.values[0];
			this->values[1] += m.values[1];
			this->values[2] += m.values[2];

			return *this;
		}
		gmat3<T>& operator-=(const gmat3<T>& m) {
			this->values[0] -= m.values[0];
			this->values[1] -= m.values[1];
			this->values[2] -= m.values[2];

			return *this;
		}
		gmat3<T>& operator*=(const gmat3<T>& m) {
			//matrix multiply. m * n != n * m;
			/**
			* gmat3<T> row(*this);
			* gmat3<T> col(m).transpose();
			* for(int i = 0; i < 3; ++i){
			*      for(int j = 0; j < 3; ++j){
			*          this->values[i][j] = row[i].dot(col[j]);
			*      }
			* }
			*/

			Row row0(this->values[0]);
			Row row1(this->values[1]);
			Row row2(this->values[2]);

			Row col0(m.values[0].x, m.values[1].x, m.values[2].x);
			Row col1(m.values[0].y, m.values[1].y, m.values[2].y);
			Row col2(m.values[0].z, m.values[1].z, m.values[2].z);

			this->values[0].x = row0.dot(col0);
			this->values[0].y = row0.dot(col1);
			this->values[0].z = row0.dot(col2);

			this->values[1].x = row1.dot(col0);
			this->values[1].y = row1.dot(col1);
			this->values[1].z = row1.dot(col2);

			this->values[2].x = row2.dot(col0);
			this->values[2].y = row2.dot(col1);
			this->values[2].z = row2.dot(col2);

			return *this;
		}

		template <typename C>
		gmat3<T>& operator+=(const C& c) {
			this->values[0] += c;
			this->values[1] += c;
			this->values[2] += c;

			return *this;
		}
		template <typename C>
		gmat3<T>& operator-=(const C& c) {
			this->values[0] -= c;
			this->values[1] -= c;
			this->values[2] -= c;

			return *this;
		}
		template <typename C>
		gmat3<T>& operator*=(const C& c) {
			this->values[0] *= c;
			this->values[1] *= c;
			this->values[2] *= c;

			return *this;
		}
		template <typename C>
		gmat3<T>& operator/=(const C& c) {
			this->values[0] /= c;
			this->values[1] /= c;
			this->values[2] /= c;

			return *this;
		}

		gmat3<T>& transpose() {
			for (int i = 0; i < 3; ++i) {
				for (int j = i + 1; j < 3; ++j) {
					swap(this->values[i][j], this->values[j][i]);
				}
			}
			
			return *this;
		}
		gmat3<T>& inverse() {
			const T cof00 = values[1][1] * values[2][2] - values[2][1] * values[1][2];
			const T cof01 = -(values[1][0] * values[2][2] - values[2][0] * values[1][2]);
			const T cof02 = values[1][0] * values[2][1] - values[2][0] * values[1][1];

			const T detM = values[0][0] * cof00 + values[0][1] * cof01 + values[0][2] * cof02;
			if (fabs(detM) <= EPSILON_E6) {
				for (auto &value : values)
					for (int j = 0; j < 3; ++j)
						value[j] = KNAN<T>;
				return *this;
			}

			const T cof10 = -(values[0][1] * values[2][2] - values[2][1] * values[0][2]);
			const T cof11 = values[0][0] * values[2][2] - values[2][0] * values[0][2];
			const T cof12 = -(values[0][0] * values[2][1] - values[2][0] * values[0][1]);\

			const T cof20 = values[0][1] * values[1][2] - values[1][1] * values[0][2];
			const T cof21 = -(values[0][0] * values[1][2] - values[1][0] * values[0][2]);
			const T cof22 = values[0][0] * values[1][1] - values[1][0] * values[0][1];

			//remember transposing matrix
			values[0][0] = cof00 / detM;
			values[0][1] = cof10 / detM;
			values[0][2] = cof20 / detM;

			values[1][0] = cof01 / detM;
			values[1][1] = cof11 / detM;
			values[1][2] = cof21 / detM;

			values[2][0] = cof02 / detM;
			values[2][1] = cof12 / detM;
			values[2][2] = cof22 / detM;

			return *this;
		}
		template <typename F = T>
		F determinant()const {
			const F cof00 = values[1][1] * values[2][2] - values[2][1] * values[1][2];
			const F cof01 = values[1][0] * values[2][2] - values[2][0] * values[1][2];
			const F cof02 = values[1][0] * values[2][1] - values[2][0] * values[1][1];

			return values[0][0] * cof00 - values[0][1] * cof01 + values[0][2] * cof02;
		}
	};

	template <typename T>
	const gmat3<T> operator-(const gmat3<T>& m) {
		return gmat3<T>(0) -= m;
	}
	template <typename T>
	const gmat3<T> operator+(const gmat3<T>& m1, const gmat3<T>& m2) {
		return gmat3<T>(m1) += m2;
	}
	template <typename T>
	const gmat3<T> operator-(const gmat3<T>& m1, const gmat3<T>& m2) {
		return gmat3<T>(m1) -= m2;
	}
	template <typename T>
	const gmat3<T> operator*(const gmat3<T>& m1, const gmat3<T>& m2) {
		return gmat3<T>(m1) *= m2;
	}

	template <typename T, typename C>
	const gmat3<T> operator+(const gmat3<T>& m, const C& c) {
		return gmat3<T>(m) += c;
	}
	template <typename T, typename C>
	const gmat3<T> operator+(const C& c, const gmat3<T>& m) {
		return gmat3<T>(m) += c;
	}
	template <typename T, typename C>
	const gmat3<T> operator-(const gmat3<T>& m, const C& c) {
		return gmat3<T>(m) -= c;
	}
	template <typename T, typename C>
	const gmat3<T> operator-(const C& c, const gmat3<T>& m) {
		return gmat3<T>(m) -= c;
	}
	template <typename T, typename C>
	const gmat3<T> operator*(const gmat3<T>& m, const C& c) {
		return gmat3<T>(m) *= c;
	}
	template <typename T, typename C>
	const gmat3<T> operator*(const C& c, const gmat3<T>& m) {
		return gmat3<T>(m) *= c;
	}
	template <typename T, typename C>
	const gmat3<T> operator/(const gmat3<T>& m, const C& c) {
		return gmat3<T>(m) /= c;
	}

	template <typename T>
	const KVector::gvec3<T> operator*(const gmat3<T>& m, const KVector::gvec3<T>& v) {
		return KVector::gvec3<T>(
			m.values[0].dot(v),
			m.values[1].dot(v),
			m.values[2].dot(v)
		);
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, gmat3<T>& m) {
		is >> m.values[0] >> m.values[1] >> m.values[2];

		return is;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const gmat3<T>& m) {
		os << m.values[0] << "\n"
		   << m.values[1] << "\n"
		   << m.values[2];

		return os;
	}

	using  mat3 = gmat3<Kfloat>;
	using dmat3 = gmat3<Kdouble>;
	using imat3 = gmat3<Kint>;
	using umat3 = gmat3<Kuint>;
	using bmat3 = gmat3<Kboolean>;
} }

#endif // !KENGINES_MAT3_H
