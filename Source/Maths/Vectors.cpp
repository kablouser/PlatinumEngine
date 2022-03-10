//
// Created by Matt on 08/03/2022.
//

#include <Maths/Vectors.h>

using namespace PlatinumEngine::Maths;

Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
Vec4::Vec4(const float x, const float y, const float z) :
	x(x), y(y), z(z), w(0.0f) {}
Vec4::Vec4(const float x, const float y, const float z, const float w) :
	x(x), y(y), z(z), w(w) {}

// Vec4 overloaders
float Vec4::operator*(const Vec4 &v)
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

Vec4 Vec4::operator-() const
{
	return Vec4(-x, -y, -z, -w);
}

Vec4& Vec4::operator+=(const Vec4 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vec4& Vec4::operator*=(const float t)
{
	x *= t;
	y *= t;
	z *= t;
	w *= t;
	return *this;
}

Vec4& Vec4::operator/=(const float t)
{
	return *this *= 1.0f/t;
}

// Vec3 constructors
Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
Vec3::Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

// Vec3 overloaders
float Vec3::operator*(const Vec3 &v)
{
	return x*v.x + y*v.y + z*v.z;
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3& Vec3::operator+=(const Vec3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3& Vec3::operator*=(const float t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

Vec3& Vec3::operator/=(const float t)
{
	return *this *= 1.0f/t;
}

// Vec2 constructors
Vec2::Vec2() : x(0.0f), y(0.0f) {}
Vec2::Vec2(const float x, const float y) : x(x), y(y) {}

// Vec2 overloaders
float Vec2::operator*(const Vec2 &v)
{
	return x*v.x + y*v.y;
}

Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}

Vec2& Vec2::operator+=(const Vec2 &v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vec2& Vec2::operator*=(const float t)
{
	x *= t;
	y *= t;
	return *this;
}

Vec2& Vec2::operator/=(const float t)
{
	return *this *= 1.0f/t;
}
