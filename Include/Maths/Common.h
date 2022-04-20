#pragma once

#include "limits"
#include "math.h"

namespace PlatinumEngine
{
	namespace Maths
	{
		class Common
		{
		public:
			static const float EPS;
			static const float PI;
			static const float DEG2RAD;
			static const float RAD2DEG;

		public:
			static float Lerp(float a, float b, float t);
		};
	}
}