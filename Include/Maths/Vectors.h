//
// Created by Matt on 08/03/2022.
//

#pragma once

#include <cmath>
#include <iostream>

namespace PlatinumEngine {
	namespace Maths {
		class Vector3 {
		public:
			Vector3();
			Vector3(float _x, float _y, float _z);

			Vector3& operator+=(const Vector3 &v);
			Vector3& operator*=(float t);
			Vector3& operator/=(float t);
			Vector3 operator-() const;

			float Length() const;
			float LengthSquared() const;
			Vector3 Unit() const;
			float Dot(const Vector3 &v) const;
			Vector3 Cross(const Vector3 &v) const;

			friend Vector3 operator*(float t, const Vector3& v);
			friend Vector3 operator*(const Vector3 &v, float t);
			friend Vector3 operator*(const Vector3 &u, const Vector3 &v);
			friend Vector3 operator/(const Vector3 &v, float t);
			friend Vector3 operator+(const Vector3 &u, const Vector3 &v);
			friend Vector3 operator-(const Vector3 &u, const Vector3 &v);
			friend std::ostream& operator<<(std::ostream &out, const Vector3 &v);

		public:
			float x, y, z;
		};

		Vector3 operator*(float t, const Vector3& v);
		Vector3 operator*(const Vector3 &v, float t);
		Vector3 operator*(const Vector3 &u, const Vector3 &v);
		Vector3 operator/(const Vector3 &v, float t);
		Vector3 operator+(const Vector3 &u, const Vector3 &v);
		Vector3 operator-(const Vector3 &u, const Vector3 &v);
		std::ostream& operator<<(std::ostream& out, const Vector3& v);
	}
}