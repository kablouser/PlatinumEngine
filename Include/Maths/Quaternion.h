#pragma once

#include <glm/glm.hpp>
#include "math.h"
#include "Maths/Vectors.h"
#include "Maths/Matrices.h"

namespace PlatinumEngine
{
	namespace Maths
	{
		class Quaternion
		{
		public:
			Quaternion();
			Quaternion(float x, float y, float z, float w);
			Quaternion(float yaw, float pitch, float roll);
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

			void Set(float newX, float newY, float newZ, float newW);
			float Length();
			float Dot(const Quaternion &q);
			Vec3 EulerAngles();
			Vec4 ToVec4();
			std::string ToString();

			static Quaternion Normalise(Quaternion &q);
			static Quaternion Conjugate(Quaternion &q);
			static Quaternion Inverse(Quaternion &q);
			static float Angle(Quaternion a, Quaternion b);
			static Quaternion Lerp(Quaternion a, Quaternion b, float t);
			static Quaternion Slerp(Quaternion a, Quaternion b, float t);
			static Vec3 QuatToEuler(Quaternion q);
			static Quaternion EulerToQuat(Vec3 euler);
			static Quaternion AngleAxis(Vec3 axis, float angle);
			static Quaternion RotationMatrix(Mat4 matrix);
			static Mat4 QuaternionToMatrix(Quaternion q);
			static Quaternion MatrixToQuaternion(Mat4 matrix);
			static Quaternion FromToRotation (Vec3 from, Vec3 to);
			static Mat4 FromToRotationMatrix (Vec3 from, Vec3 to);
			static Quaternion LookRotation(Vec3 forward, Vec3 up);
			static Quaternion LookRotation(Vec3 forward);

		public:
			static Quaternion identity;
			float x,y,z,w;

		private:
			static bool LookRotationToQuaternion (Vec3 viewVec, Vec3 upVec, Quaternion& q);
			static bool LookRotationToMatrix (Vec3 viewVec, Vec3 upVec,Mat4& m);
			static Mat4 SetBasis(Vec3 inX, Vec3 inY, Vec3 inZ);
			static bool Approximately(float a, float b);

		private:
			static Vec3 _forward;
			static Vec3 _up;
			static Vec3 _right;
			static float _eps;

		};
	}
}
