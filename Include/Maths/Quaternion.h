#pragma once

#include <glm/glm.hpp>
#include "Maths/Vectors.h"
#include "Maths/Matrices.h"
#include "ostream"

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
			// Vec3 (Pitch,Yaw,Roll) in degrees
			Vec3 EulerAngles();
			// in glm::quat format
			Vec4 ToVec4();

			//Returns the quaternion of unit length
			static Quaternion Normalise(Quaternion &q);
			//Returns the conjugate of quaternion
			static Quaternion Conjugate(Quaternion &q);
			//Returns the inverse of quaternion
			static Quaternion Inverse(Quaternion &q);
			//Returns the angle (radians) between 2 quaternions
			static float Angle(Quaternion a, Quaternion b);
			//Linear interpolation of 2 quaternions
			static Quaternion Lerp(Quaternion a, Quaternion b, float t);
			//Spherical interpolation of 2 quaternions
			static Quaternion Slerp(Quaternion a, Quaternion b, float t);
			//Creates a quaternion with the given axis and angle values
			static Quaternion AngleAxis(Vec3 axis, float angle);
			//Returns a Vec3 (Pitch,Yaw,Roll) euler angles (degrees) from a Quaternion
			static Vec3 QuaternionToEuler(Quaternion q);
			//Returns a Quaternion from Vec3 (Pitch,Yaw,Roll) euler angles (degrees)
			static Quaternion EulerToQuaternion(Vec3 euler);
			//Returns a rotation matrix from a Quaternion
			static Mat4 QuaternionToMatrix(Quaternion q);
			//Returns a Quaternion from a rotation matrix
			static Quaternion MatrixToQuaternion(Mat4 matrix);
			//Returns a Vec3 (Pitch,Yaw,Roll) euler angles (degrees) from a rotation matrix
			static Vec3 MatrixToEuler(Mat4 matrix);
			//Returns a rotation matrix from Vec3 euler angles (Pitch,Yaw,Roll) in degrees
			static Mat4 EulerToMatrix(Vec3 euler);
			static Quaternion FromToRotation (Vec3 from, Vec3 to);
			//Creates a Quaternion with the specified forward and up directions
			static Quaternion LookRotation(Vec3 forward, Vec3 up = Vec3(0,1,0));

		public:
			static Quaternion identity;
			float x,y,z,w;

		};
		std::ostream& operator <<(std::ostream& os, Quaternion& q);
	}
}
