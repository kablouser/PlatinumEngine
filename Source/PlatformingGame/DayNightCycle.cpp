#include <Application.h>
#include <PlatformingGame/DayNightCycle.h>
#include <Logger/Logger.h>
#include <Maths/Common.h>

namespace PlatinumEngine
{
	void DayNightCycle::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<DayNightCycle>()
		        .WithField<Maths::Vec3>("dayColor", PLATINUM_OFFSETOF(DayNightCycle, dayColor))
				.WithField<Maths::Vec3>("nightColor", PLATINUM_OFFSETOF(DayNightCycle, nightColor))
				.WithField<float>("wholeDayDuration", PLATINUM_OFFSETOF(DayNightCycle, wholeDayDuration));
	}

	void DayNightCycle::OnStart()
	{
		_transform = GetComponent<Transform>();
		_light = GetComponent<LightComponent>();
	}

	void DayNightCycle::OnUpdate()
	{
		if (!_transform || !_light)
		{
			PLATINUM_WARNING("Setup properly please.");
			return;
		}

		float seconds = Application::Instance->time.getSeconds();
		int days = std::floor(seconds / wholeDayDuration);
		// fraction [0,1]
		float currentDay = (seconds - days * wholeDayDuration) / wholeDayDuration;
		// fraction [0,2]
		float currentPeriod = currentDay * 2;
		bool isDay = true;
		// back to faction [0,1]. But double the frequency.
		if (1 < currentPeriod)
		{
			currentPeriod -= 1;
			isDay = false;
		}

		_transform.DeRef()->localRotation =
				Maths::Quaternion::EulerToQuaternion({0,0, 180.f + currentPeriod * 180.f}) *
				Maths::Quaternion::EulerToQuaternion({0,90.f,0});

		bool isFirstQuarter = true;
		float currentQuarter = currentPeriod * 2;
		if (1 < currentQuarter)
		{
			currentQuarter -= 1;
			isFirstQuarter = false;
		}

		Maths::Vec3 twilightColor = (dayColor + nightColor) / 2.0f;

		for (int i = 0; i < 3; ++i)
		{
			if (isDay)
			{
				_light.DeRef()->spectrum.data[i] = dayColor[i];
//				if (isFirstQuarter)
//					_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(twilightColor[i], dayColor[i], currentPeriod);
//				else
//					_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(dayColor[i], twilightColor[i], currentPeriod);
			}
			else
			{
				_light.DeRef()->spectrum.data[i] = {};// nightColor[i];
//				if (isFirstQuarter)
//					_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(twilightColor[i], nightColor[i], currentPeriod);
//				else
//					_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(nightColor[i], twilightColor[i], currentPeriod);
			}
		}
	}
}