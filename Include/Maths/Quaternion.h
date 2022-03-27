#ifndef PLATINUMENGINE_QUATERNION_H
#define PLATINUMENGINE_QUATERNION_H

#include <glm/glm.hpp>
#include "math.h"
#include "Maths/Vectors.h"

namespace PlatinumEngine
{
	namespace Maths
	{
		class Quaternion
		{
		public:
			Quaternion();
			Quaternion(float x, float y, float z, float w);
			~Quaternion();

			void operator *=(const Quaternion &q);
			void operator *=(const float value);
			void operator *=(const int value);
			void operator +=(const Quaternion &q);
			void operator -=(const Quaternion &q);
			Quaternion operator *(const Quaternion &q);
			Quaternion operator *(const float value);
			Quaternion operator *(const int value);
			Quaternion operator +(const Quaternion &q);
			Quaternion operator -(const Quaternion &q);
			void operator =(const Quaternion &q);
			bool operator ==(const Quaternion &q);

			void Set(float newX, float newY, float newZ, float newW);
			float Norm();
			float Dot(const Quaternion &q);
			Vec3 EulerAngles();

			static Quaternion Normalise(Quaternion &q);
			static Quaternion Conjugate(Quaternion &q);
			static Quaternion Inverse(Quaternion &q);
			static float Angle(Quaternion a, Quaternion b);
			static Quaternion Slerp(Quaternion a, Quaternion b, float t);
			static Vec3 QuatToEuler(Quaternion q);
			static Quaternion EulerToQuat(Vec3 euler);
			static Quaternion AngleAxis(Vec3 axis, float angle);

		public:
			static Quaternion identity;
			float x,y,z,w;
		};
	}
}
#endif //PLATINUMENGINE_QUATERNION_H
