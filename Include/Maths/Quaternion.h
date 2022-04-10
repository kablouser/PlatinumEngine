#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "math.h"
#include "Maths/Vectors.h"
#include "Maths/Matrices.h"
#include "ostream"
#include "Maths/Common.h"

namespace PlatinumEngine
{
	namespace Maths
	{
		class Quaternion
		{
		public:
			Quaternion();
			Quaternion(float w, float x, float y, float z);
			Quaternion(float pitch, float yaw, float roll);
			Quaternion(Vec3 euler);
			~Quaternion();

			void operator *=(const Quaternion &q);
			void operator *=(const float value);
			void operator *=(const int value);
			void operator +=(const Quaternion &q);
			void operator -=(const Quaternion &q);
			Quaternion operator *(const Quaternion &q);
			Quaternion operator *(const float value);
			Quaternion operator *(const int value);
			Vec3 operator *(const Vec3 v);
			Quaternion operator +(const Quaternion &q);
			Quaternion operator -(const Quaternion &q);
			void operator =(const Quaternion &q);
			bool operator ==(const Quaternion &q);
			float& operator[](int i);

			void Set(float newW, float newX, float newY, float newZ);
			float Length();
			float Dot(const Quaternion &q);
			Vec3 EulerAngles();
			Vec4 ToVec4();

			static Quaternion Normalise(Quaternion &q);
			static Quaternion Conjugate(Quaternion &q);
			static Quaternion Inverse(Quaternion &q);
			static float Angle(Quaternion a, Quaternion b);
			static Quaternion Lerp(Quaternion a, Quaternion b, float t);
			static Quaternion Slerp(Quaternion a, Quaternion b, float t);
			static Quaternion AngleAxis(Vec3 axis, float angle);
			static Vec3 QuaternionToEuler(Quaternion q);
			static Quaternion EulerToQuaternion(Vec3 euler);
			static Mat4 QuaternionToMatrix(Quaternion q);
			static Quaternion MatrixToQuaternion(Mat4 matrix);
			static Vec3 MatrixToEuler(Mat4 matrix);
			static Mat4 EulerToMatrix(Vec3 euler);
			static Quaternion FromToRotation (Vec3 from, Vec3 to);
			static Quaternion LookRotation(Vec3 forward, Vec3 up);
			static Quaternion LookRotation(Vec3 forward);

		public:
			static Quaternion identity;
			float x,y,z,w;

		};
		std::ostream& operator <<(std::ostream& os, Quaternion& q);
	}
}
