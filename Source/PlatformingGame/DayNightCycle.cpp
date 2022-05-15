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


		/*
		 * Go from 0 -> 180 twice per day
		 * First time isDay = true
		 * */
		_transform.DeRef()->localRotation = Maths::Quaternion::EulerToQuaternion({0,0,currentDay*360});

		bool isFirstQuarter = true;
		float currentQuarter = currentPeriod * 2;
		if (1 < currentQuarter)
		{
			currentQuarter -= 1;
			isFirstQuarter = false;
		}

		Maths::Vec3 twilightColor = (dayColor + nightColor) / 2.0f;

		/**
		 * At time 0 -> sunrise/sunset colour
		 * At time 0.25 -> day colour
		 * At time 0.5 -> sunrise/sunset colour
		 * At time 0.75 -> night colour
		 * At time 1 -> sunrise/sunset colour
		 */

		auto OldRange = 0.25;
		auto NewRange = 1.0f;

		for (int i = 0; i < 3; ++i)
		{
			if (0.0f <= currentDay && currentDay < 0.25f)
			{
				auto newValue = (((currentDay - 0.0f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(twilightColor[i], dayColor[i], newValue);
			}

			if (0.25f <= currentDay && currentDay < 0.5f)
			{
				auto newValue = (((currentDay - 0.25f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(dayColor[i], twilightColor[i], newValue);
			}

			if (0.5f <= currentDay && currentDay < 0.75f)
			{
				auto newValue = (((currentDay - 0.5f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(twilightColor[i], nightColor[i], newValue);
			}

			if (0.75f <= currentDay && currentDay <= 1.0f)
			{
				auto newValue = (((currentDay - 0.75f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(nightColor[i], twilightColor[i], newValue);
			}
		}
	}
}