//
// Created by KingSun on 2018/06/30
//

#ifndef KENGINES_QUATERNION
#define KENGINES_QUATERNION

#include "mat4.h"

namespace KEngines { namespace KMatrix {
	using namespace KVector;

	template <typename T>
	struct gquaternion {
		//normalized quaternion: w*w + x*x + y*y + z*z = 1
		template <typename FT>
		friend const gvec3<FT> operator*(const gquaternion<FT>& q, const gvec3<FT>& v);
		template <typename FT>
		friend std::istream& operator>>(std::istream& is, gquaternion<FT>& q);
		template <typename FT>
		friend std::ostream& operator<<(std::ostream& os, const gquaternion<FT>& q);
		
	private:
		T w, x, y, z;

		gquaternion(const T& w, const T& x, const T& y, const T& z):
					w(w), x(x), y(y), z(z) {}

	public:
		gquaternion() : w(T(1)), x(T(0)), y(T(0)), z(T(0)) {} //rotate zero degree
		gquaternion(const T& angle, const gvec3<T>& v) { set(angle, v); }
		gquaternion(const gquaternion<T>& q) : w(q.w), x(q.x), y(q.y), z(q.z) {}

		Kboolean isValid() {
			return  abs(w*w + x*x + y*y + z*z - static_cast<T>(1))
					< static_cast<T>(EPSILON_E6);
		}
		Kboolean operator==(const gquaternion<T>& q)const {
			return this->w == q.w
				&& this->x == q.x
				&& this->y == q.y
				&& this->z == q.z;
		}
		Kboolean operator!=(const gquaternion<T>& q)const {
			return !(this->operator==(q));
		}

		void set(const T& w, const T& x, const T& y, const T& z) {
			if (abs(w * w + x * x + y * y + z * z - 1) >= EPSILON_E6) {
				Log::error("The parameters do not match the unit quaternion! Set to default!");
				setDefault();
				return;
			}
			this->w = w;
			this->x = x;
			this->y = y;
			this->z = z;
		}
		void set(const T& angle, const gvec3<T>& v) {
			gvec3<T> tv(v);
			tv.normalize();

			const auto half = toRadian<T>(angle / static_cast<T>(2));
			const auto s = static_cast<T>(sin(half));
			this->w = static_cast<T>(cos(half));
			this->x = tv.x * s;
			this->y = tv.y * s;
			this->z = tv.z * s;
		}
		void setDefault() {
			const auto zero = static_cast<T>(0);
			const auto one = static_cast<T>(1);

			this->w = one;
			this->x = zero;
			this->y = zero;
			this->z = zero;
		}

		gquaternion<T>& operator=(const gquaternion<T>& q) {
			this->w = q.w;
			this->x = q.x;
			this->y = q.y;
			this->z = q.z;

			return *this;
		}
		gquaternion<T>& operator*=(const gquaternion<T>& q) {
			//i*i = j*j = k*k = ijk = -1
			//ij = k, ki = -k;
			//jk = i, kj = -i;
			//ki = j, ik = -j;

			const T tw = this->w;
			const T tx = this->x;
			const T ty = this->y;
			const T tz = this->z;

			this->w = tw * q.w - tx * q.x - ty * q.y - tz * q.z;
			this->x = tw * q.x + tx * q.w + ty * q.z - tz * q.y;
			this->y = tw * q.y - tx * q.z + ty * q.w + tz * q.x;
			this->z = tw * q.z + tx * q.y - ty * q.x + tz * q.w;

			return *this;
		}
		gquaternion<T>& inverse() {
			this->x = -x;
			this->y = -y;
			this->z = -z;

			return *this;
		}

		const gquaternion<T> getConjugate()const {
			return gquaternion<T>(w, -x, -y, -z);
		}
		const T getAngle()const {
			return acos(w) * 2;
		}
		const gvec3<T> getAxis()const {
			return gvec3<T>(x, y, z).normalize();
		}

		gquaternion<T>& fromMatrix(const gmat3<T>& m) {
			T w_divisor = m[0][0] + m[1][1] + m[2][2];
			T x_divisor = m[0][0] - m[1][1] - m[2][2];
			T y_divisor = m[1][1] - m[0][0] - m[2][2];
			T z_divisor = m[2][2] - m[0][0] - m[1][1];

			Kuint index = 0;
			T divisor = w_divisor;
			if (divisor < x_divisor) {
				divisor = x_divisor;
				index = 1;
			}
			if (divisor < y_divisor) {
				divisor = y_divisor;
				index = 2;
			}
			if (divisor < z_divisor) {
				divisor = z_divisor;
				index = 3;
			}

			auto factor = static_cast<T>(std::sqrt(divisor + 1.0) * 0.5);
			divisor = static_cast<T>(0.25) / factor;

			switch (index) {
			case 0:
				set(factor, (m[2][1] - m[1][2]) * divisor,
					(m[0][2] - m[2][0]) * divisor, (m[1][0] - m[0][1]) * divisor);
				break;
			case 1:
				set((m[2][1] - m[1][2]) * divisor, factor,
					(m[1][0] + m[0][1]) * divisor, (m[0][2] + m[2][0]) * divisor);
				break;
			case 2:
				set((m[0][2] - m[2][0]) * divisor, (m[1][0] + m[0][1]) * divisor,
					factor, (m[2][1] + m[1][2]) * divisor);
				break;
			case 3:
				set((m[1][0] - m[0][1]) * divisor, (m[0][2] + m[2][0]) * divisor,
					(m[2][1] + m[1][2]) * divisor, factor);
				break;
			}

			return *this;
		}
		gquaternion<T>& fromMatrix(const gmat4<T>& m) {
			return fromMatrix(m.toMat3());
		}
		gquaternion<T>& fromVectors(const gvec3<T>& from_vector, const gvec3<T>& to_vector) {
			set(from_vector.getAngle(to_vector), gvec3<T>::cross(from_vector, to_vector));

			return *this;
		}

		const gmat3<T> toMat3()const {
			T w2 = w * w;
			T x2 = x * x;
			T y2 = y * y;
			T z2 = z * z;

			T wx = w * x;
			T wy = w * y;
			T wz = w * z;

			T xy = x * y;
			T xz = x * z;
			T yz = y * z;

			return gmat3<T>(
				w2 + x2 - y2 - z2, 2 * (xy - wz), 2 * (xz + wy),
				2 * (xy + wz), w2 - x2 + y2 - z2, 2 * (yz - wx),
				2 * (xz - wy), 2 * (yz + wx), w2 - x2 - y2 + z2
			);
		}
		const gmat4<T> toMat4()const {
			T w2 = w * w;
			T x2 = x * x;
			T y2 = y * y;
			T z2 = z * z;

			T wx = w * x;
			T wy = w * y;
			T wz = w * z;

			T xy = x * y;
			T xz = x * z;
			T yz = y * z;

			return gmat4<T>(
				w2 + x2 - y2 - z2, 2 * (xy - wz), 2 * (xz + wy), 0,
				2 * (xy + wz), w2 - x2 + y2 - z2, 2 * (yz - wx), 0,
				2 * (xz - wy), 2 * (yz + wx), w2 - x2 - y2 + z2, 0,
				0, 0, 0, 1
			);
		}
	};

	template <typename T>
	const gquaternion<T> operator-(const gquaternion<T>& q) {
		//note: just get its conjugate!
		return q.getConjugate();
	}

	template <typename T>
	const gquaternion<T> operator*(const gquaternion<T>& q1, const gquaternion<T>& q2) {
		return gquaternion<T>(q1) *= q2;
	}

	template <typename T>
	const gvec3<T> operator*(const gquaternion<T>& q, const gvec3<T>& v) {
		gquaternion<T> p = (q * gquaternion<T>(0, v.x, v.y, v.z)) * q.getConjugate();
		//do not use gquaternion<T>(angle, v) because the vector will be normalized.
		return gvec3<T>(p.x, p.y, p.z);
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, gquaternion<T>& q) {
		is >> q.w >> q.x >> q.y >> q.z;
		return is;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const gquaternion<T>& q) {
		os << q.w << " " << q.x << " " << q.y << " " << q.z;
		return os;
	}

	using  quaternion = gquaternion<Kfloat>;
	using dquaternion = gquaternion<Kdouble>;
} }

#endif // !KENGINES_QUATERNION
