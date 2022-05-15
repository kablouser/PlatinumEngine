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
				.WithField<Maths::Vec3>("twilightColor", PLATINUM_OFFSETOF(DayNightCycle, twilightColor))
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
		float currentDay = 1.0f - (seconds - days * wholeDayDuration) / wholeDayDuration;
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
		_transform.DeRef()->localRotation = Maths::Quaternion::EulerToQuaternion({10,10,currentDay*360});

		bool isFirstQuarter = true;
		float currentQuarter = currentPeriod * 2;
		if (1 < currentQuarter)
		{
			currentQuarter -= 1;
			isFirstQuarter = false;
		}

		/**
		 * At time 0 -> sunrise/sunset colour
		 * At time 0.25 -> day colour
		 * At time 0.5 -> sunrise/sunset colour
		 * At time 0.75 -> night colour
		 * At time 1 -> sunrise/sunset colour
		 */

		float OldRange = 0.25f;
		float NewRange = 1.0f;

		for (int i = 0; i < 3; ++i)
		{
			if (0.0f <= currentDay && currentDay < 0.25f)
			{
				// SKybox 0.5 -> 1
				float newValue = (((currentDay - 0.0f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(twilightColor[i], dayColor[i], newValue);
				// Update sky box multiplier
//				Application::Instance->renderer._skyBoxShader.Bind();
//				Application::Instance->renderer._skyBoxShader.SetUniform("intensity", 0.5f+(newValue/2.0f));
//				Application::Instance->renderer.Begin();
			}

			if (0.25f <= currentDay && currentDay < 0.5f)
			{
				// SKybox 1 -> 0.5
				float newValue = (((currentDay - 0.25f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(dayColor[i], twilightColor[i], newValue);
				// Update sky box multiplier
//				Application::Instance->renderer._skyBoxShader.Bind();
//				Application::Instance->renderer._skyBoxShader.SetUniform("intensity", 1.0f-(newValue/2.0f));
//				Application::Instance->renderer.Begin();
			}

			if (0.5f <= currentDay && currentDay < 0.75f)
			{
				// SKybox 0.5 -> 0.0
				float newValue = (((currentDay - 0.5f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(twilightColor[i], nightColor[i], newValue);
				// Update sky box multiplier
//				Application::Instance->renderer._skyBoxShader.Bind();
//				Application::Instance->renderer._skyBoxShader.SetUniform("intensity", 0.5f - (newValue/2.0f));
//				Application::Instance->renderer.Begin();
			}

			if (0.75f <= currentDay && currentDay <= 1.0f)
			{
				// SKybox 0.0 -> 0.5
				float newValue = (((currentDay - 0.75f) * NewRange) / OldRange) + 0.0f;
				_light.DeRef()->spectrum.data[i] = Maths::Common::Lerp(nightColor[i], twilightColor[i], newValue);
				// Update sky box multiplier
//				Application::Instance->renderer._skyBoxShader.Bind();
//				Application::Instance->renderer._skyBoxShader.SetUniform("intensity", 0.0f + (newValue/2.0f));
//				Application::Instance->renderer.Begin();
			}
		}

		Application::Instance->renderer._skyBoxShader.Bind();
		Application::Instance->renderer._skyBoxShader.SetUniform("colour",
				Maths::Vec3{_light.DeRef()->spectrum.data[0], _light.DeRef()->spectrum.data[1], _light.DeRef()->spectrum.data[2]});
		Application::Instance->renderer.Begin();
	}

	void DayNightCycle::OnEnd()
	{
		Application::Instance->renderer._skyBoxShader.Bind();
		Application::Instance->renderer._skyBoxShader.SetUniform("colour",
				Maths::Vec3{1,1,1});
		Application::Instance->renderer.Begin();
	}
}