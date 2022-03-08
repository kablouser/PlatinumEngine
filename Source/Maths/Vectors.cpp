//
// Created by Matt on 08/03/2022.
//

#include <Maths/Vectors.h>

using namespace PlatinumEngine::Maths;

/* === Vector3 === */
// Constructors
Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
Vector3::Vector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator*=(const float t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

Vector3& Vector3::operator/=(const float t)
{
	return *this *= 1.0f/t;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

float Vector3::Length() const
{
	return std::sqrt(LengthSquared());
}

float Vector3::LengthSquared() const
{
	return x*x + y*y + z*z;
}

Vector3 Vector3::Unit() const
{
	float length = Length();
	return Vector3(x/length, y/length, z/length);
}

float Vector3::Dot(const Vector3& v) const
{
	return x*v.x + y*v.y + z*v.z;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3(
				y*v.z - z*v.y,
				z*v.x - x*v.z,
				x*v.y - y*v.x
			);
}

Vector3 operator*(float t, const Vector3& v)
{
	return Vector3(t*v.x, t*v.y, t*v.z);
}

Vector3 operator*(const Vector3 &v, float t)
{
	return Vector3(t*v.x, t*v.y, t*v.z);
}

Vector3 operator*(const Vector3 &u, const Vector3 &v)
{
	Vector3(u.x*v.x, u.y*v.y, u.z*v.z);
}

Vector3 operator/(const Vector3 &v, float t)
{
	return Vector3(1.0f/t *v.x, 1.0f/t *v.y, 1.0f/t *v.z);
}

Vector3 operator+(const Vector3 &u, const Vector3 &v)
{
	return Vector3(u.x+v.x, u.y+v.y, u.z+v.z);
}

Vector3 operator-(const Vector3 &u, const Vector3 &v)
{
	return Vector3(u.x-v.x, u.y-v.y, u.z-v.z);

}

std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << v.x << ' ' << v.y << ' ' << v.z;
}

/* === Vector3 end === */