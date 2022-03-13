//
// Created by Matt on 08/03/2022.
//

#include <Maths/Vectors.h>

using namespace PlatinumEngine::Maths;



Vec4::Vec4() : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) {}
Vec4::Vec4(const float x, const float y, const float z) :
	glm::vec4(x, y, z, 0.0f) {}
Vec4::Vec4(const float x, const float y, const float z, const float w) :
	glm::vec4(x, y, z, w) {}

namespace PlatinumEngine
{
	namespace Maths
	{
		std::ostream& operator<<(std::ostream &out, const Vec4 &v)
		{
			return out << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
		}
	}
}

// Vec4 overloaders
Vec4 Vec4::operator-() const
{
	return {-x, -y, -z, -w};
}

Vec4& Vec4::operator+=(const Vec4 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vec4& Vec4::operator-=(const Vec4 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
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
Vec3::Vec3() : glm::vec3(0.0f, 0.0f, 0.0f) {}
Vec3::Vec3(const float x, const float y, const float z) : glm::vec3(x, y, z) {}

namespace PlatinumEngine
{
	namespace Maths
	{
		std::ostream& operator<<(std::ostream &out, const Vec3 &v)
		{
			return out << v.x << ' ' << v.y << ' ' << v.z;
		}
	}
}


//// Vec3 overloaders


//float Vec3::operator*(const Vec3 &v)
//{
//	return x*v.x + y*v.y + z*v.z;
//}


Vec3 Vec3::operator-() const
{
	return {-x, -y, -z};
}

Vec3& Vec3::operator+=(const Vec3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
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
Vec2::Vec2() : glm::vec2(0.0f, 0.0f) {}
Vec2::Vec2(const float x, const float y) : glm::vec2(x, y) {}

namespace PlatinumEngine
{
	namespace Maths
	{
		std::ostream& operator<<(std::ostream &out, const Vec2 &v)
		{
			return out << v.x << ' ' << v.y;
		}
	}
}

// Vec2 overloaders
Vec2 Vec2::operator-() const
{
	return {-x, -y};
}

Vec2& Vec2::operator+=(const Vec2 &v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2 &v)
{
	x -= v.x;
	y -= v.y;
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

