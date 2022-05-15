#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/Light.h>
#include <TypeDatabase/TypeDatabase.h>
#include <Maths/Vectors.h>

namespace PlatinumEngine
{
	class DayNightCycle : public Component
	{
	public:

		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		void OnStart() override;

		void OnUpdate() override;

		Maths::Vec3 dayColor = {1.0,1.0,0.0}, nightColor = {0.0,0.1,0.4};
		// in seconds
		float wholeDayDuration = 20.f;

	private:

		SavedReference<Transform> _transform;
		SavedReference<LightComponent> _light;
	};
}