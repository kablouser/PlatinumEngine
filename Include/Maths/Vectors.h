//
// Created by Matt on 08/03/2022.
//

#pragma once

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

// TODO:
// Potentially add reflection, refraction and faceforward from glm if needed

namespace PlatinumEngine {
	namespace Maths {
		class Vec4 : public glm::vec4 {
		public:
			Vec4();
			Vec4(float x, float y, float z);
			Vec4(float x, float y, float z, float w);
      
			Vec4 operator-() const;
			Vec4& operator+=(const Vec4 &v);
			Vec4& operator-=(const Vec4 &v);
			Vec4& operator*=(float t);
			Vec4& operator/=(float t);
			friend std::ostream& operator<<(std::ostream &out, const Vec4 &v);
			template<typename T> friend float Dot(T &u, T &v);
			template<typename T> friend float Length(T &v);
			template<typename T> friend float Distance(T &u, T &v);

		private:
			float DotProduct(const Vec4 &u)
			{
				return glm::dot(glm::vec4(*this), u);
			}
			float CalcLength()
			{
				return glm::length(glm::vec4(*this));
			}
			float CalcDistance(const Vec4 &u)
			{
				return glm::distance(glm::vec4(*this), u);
			}
		};

		std::ostream& operator<<(std::ostream &out, const Vec4 &v);

		/**
		 * Calculates the square of the length of a vector size 4
		 * @param v Input vector
		 * @return Square of length
		 */
		inline float LengthSquared(const Vec4 &v) { return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w; }

		// Some more operator overloading for a Vec4
		inline Vec4 operator*(const float t, const Vec4 &v) { return {t*v.x, t*v.y, t*v.z, t*v.w}; }
		inline Vec4 operator*(const Vec4 &v, const float t) { return t*v; }
		inline Vec4 operator*(const Vec4 &u, const Vec4 &v)
		{
			return {u.x*v.x, u.y*v.y, u.z*v.z, u.w*v.w};
		}
		inline Vec4 operator/(const Vec4 &v, const float t) { return (1.0f/t) * v; }
		inline Vec4 operator+(const Vec4 &u, const Vec4 &v)
		{
			return {u.x+v.x, u.y+v.y, u.z+v.z, u.w+v.w};
		}
		inline Vec4 operator-(const Vec4 &u, const Vec4 &v)
		{
			return {u.x-v.x, u.y-v.y, u.z-v.z, u.w-v.w};
		}

		/**
		 * Data structure for a vector of three values
		 */
		class Vec3 : public glm::vec3 {
		public:
			static Vec3 forward;
			static Vec3 up;
			static Vec3 right;
		public:
			Vec3();
			Vec3(float x, float y, float z);
			Vec3 operator-() const;
			Vec3& operator+=(const Vec3 &v);
			Vec3& operator-=(const Vec3 &v);
			Vec3& operator*=(float t);
			Vec3& operator/=(float t);
			Vec3& unit();
			bool operator==(Vec3 v) const;
			bool operator!=(Vec3 v) const;
			friend std::ostream& operator<<(std::ostream &out, const Vec3 &v);
			template<typename T> friend float Dot(T &u, T &v);
			template<typename T> friend float Length(T &v);
			template<typename T> friend float Distance(T &u, T &v);

		private:
			float DotProduct(const Vec3 &u)
			{
				return glm::dot(glm::vec3(*this), u);
			}
			float CalcLength()
			{
				return glm::length(glm::vec3(*this));
			}
			float CalcDistance(const Vec3 &u)
			{
				return glm::distance(glm::vec3(*this), u);
			}
		};

		std::ostream& operator<<(std::ostream &out, const Vec3 &v);

		/**
		 * Calculates the square of the length of a vector size 3
		 * @param v Input vector
		 * @return Square of length
		 */
		inline float LengthSquared(const Vec3 &v) { return v.x*v.x + v.y*v.y + v.z*v.z; }

		// Some more operator overloading for a Vec3
		inline Vec3 operator*(const float t, const Vec3 &v) { return {t*v.x, t*v.y, t*v.z}; }
		inline Vec3 operator*(const Vec3 &v, const float t) { return t*v; }
		inline Vec3 operator*(const Vec3 &u, const Vec3 &v)
		{
			return {u.x * v.x, u.y * v.y, u.z * v.z};
		}
		inline Vec3 operator/(const Vec3 &v, const float t) { return (1.0f/t) * v; }
		inline Vec3 operator+(const Vec3 &u, const Vec3 &v)
		{
			return {u.x + v.x, u.y + v.y, u.z + v.z};
		}
		inline Vec3 operator-(const Vec3 &u, const Vec3 &v)
		{
			return {u.x - v.x, u.y - v.y, u.z - v.z};
		}
		inline bool operator<(Vec3 l, Vec3 r) {
			if(l.x == r.x) {
				if(l.y == r.y) {
					return l.z < r.z;
				}
				return l.y < r.y;
			}
			return l.x < r.x;
		}
		/**
		 * Calculates the cross product of two vectors
		 * @param u First input vector
		 * @param v Second input vector
		 * @return The cross product of the two input vectors
		 */
		inline Vec3 Cross(const Vec3 &u, const Vec3 &v)
		{
			return {u.y * v.z - u.z * v.y,
					u.z * v.x - u.x * v.z,
					u.x * v.y - u.y * v.x};
		}

		/**
		 * Data structure for a vector with two values
		 */
		class Vec2 : public glm::vec2 {
		public:
			Vec2();
			Vec2(float x, float y);
			Vec2 operator-() const;
			Vec2& operator+=(const Vec2 &v);
			Vec2& operator-=(const Vec2 &v);
			Vec2& operator*=(float t);
			Vec2& operator/=(float t);
			friend std::ostream& operator<<(std::ostream &out, const Vec2 &v);
			template<typename T> friend float Dot(T &u, T &v);
			template<typename T> friend float Length(T &v);
			template<typename T> friend float Distance(T &u, T &v);

		private:
			float DotProduct(const Vec2 &u)
			{
				return glm::dot(glm::vec2(*this), u);
			}
			float CalcLength()
			{
				return glm::length(glm::vec2(*this));
			}
			float CalcDistance(const Vec2 &u)
			{
				return glm::distance(glm::vec2(*this), u);
			}
		};

		std::ostream& operator<<(std::ostream &out, const Vec2 &v);

		/**
		 * Calculates the square of the length of a vector size 2
		 * @param v Input vector
		 * @return Square of length
		 */
		inline float LengthSquared(const Vec2 &v) { return v.x*v.x + v.y*v.y; }

		// Some more operator overloading for a Vec2
		inline Vec2 operator*(const float t, const Vec2 &v) { return {t*v.x, t*v.y}; }
		inline Vec2 operator*(const Vec2 &v, const float t) { return t*v; }
		inline Vec2 operator*(const Vec2 &u, const Vec2 &v)
		{
			return {u.x * v.x, u.y * v.y};
		}
		inline Vec2 operator/(const Vec2 &v, const float t) { return (1.0f/t) * v; }
		inline Vec2 operator+(const Vec2 &u, const Vec2 &v)
		{
			return {u.x + v.x, u.y + v.y};
		}
		inline Vec2 operator-(const Vec2 &u, const Vec2 &v)
		{
			return {u.x - v.x, u.y - v.y};
		}


		/**
		 * Calculates the length of the vector
		 * @tparam T Type of vector
		 * @param v Input vector
		 * @return Length as float
		 */
		template<typename T> float Length(T &v)
		{
			return v.CalcLength();
		}

		/**
		 * Calculates the dot product of two vectors. They must be the same type.
		 * @tparam T Type of vectors
		 * @param u First input vector
		 * @param v Second input vector
		 * @return Dot product of the two
		 */
		template<typename T> float Dot(T &u, T &v)
		{
			return u.DotProduct(v);
		}

		/**
		 * Normalises a vector to length 1
		 * @tparam T Type of vector
		 * @param v Input vector
		 * @return A vector of length 1
		 */
		template<typename T> T Normalise(T &v)
		{
			return v / Length(v);
		}

		/**
		 * Calculates the distance between u and v (i.e. Length(u-v))
		 * @tparam T Type of vectors
		 * @param u First input vector
		 * @param v Second input vector
		 * @return Distance between the two vectors as a float
		 */
		template<typename T> float Distance(T &u, T &v)
		{
			return u.CalcDistance(v);
		}
	}
}