#include <Maths/Common.h>

namespace PlatinumEngine
{
	namespace Maths
	{
		float Common::Lerp(float a, float b, float t)
		{
			return a+t*(b-a);
		}

		const float Common::EPS = std::numeric_limits<float>::epsilon();
		const float Common::PI = M_PI;
		const float Common::DEG2RAD = PI / 180.f;
		const float Common::RAD2DEG = 180.f / PI;

		float Random::GetRandom(float min, float max, bool useNormalDistribution)
		{
			return Instance().GetRandomInternal(min, max, useNormalDistribution);
		}

		float Random::GetRandomInternal(float min, float max, bool useNormalDistribution)
		{
			if (useNormalDistribution)
			{
				// Mean is assumed to be middle of min and max, and assume min/max are 3 standard deviations away from mean
				std::normal_distribution<float> distribution{(min + max) / 2, (max - min) / 6};
				while (true)
				{
					float number = distribution(_randomEngine);
					if (number >= min && number <= max)
						return number;
				}
			}

			// Uniform distribution
			std::uniform_real_distribution<float> dist(min, std::nextafter(max, FLT_MAX));
			return dist(_randomEngine);
		}
	}
}