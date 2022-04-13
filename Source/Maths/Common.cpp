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
	}

}