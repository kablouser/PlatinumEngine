#pragma once

#include "limits"
#include "math.h"
#include <random>
#include <cfloat>

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

		class Random {
		public:
			Random(const Random&) = delete;

			static Random& Instance()
			{
				static Random instance;
				return instance;
			}

			static float GetRandom(float min, float max, bool useNormalDistribution = false);

		private:
			Random() = default;
			float GetRandomInternal(float min, float max, bool useNormalDistribution);
		private:
			std::default_random_engine _randomEngine;
		};
	}
}