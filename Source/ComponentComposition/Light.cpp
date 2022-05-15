//
// Created by Jinyuan on 04/04/2022.
//
#include "ComponentComposition/Light.h"
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/Objects.h>
#include <Application.h>
#include <OpenGL/Mesh.h>

namespace PlatinumEngine
{
	void LightComponent::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<LightComponent>()
		        .WithField<LightType>("type", PLATINUM_OFFSETOF(LightComponent, type))
				.WithField<Spectrum>("spectrum", PLATINUM_OFFSETOF(LightComponent, spectrum))
				.WithField<Maths::Vec3>("direction", PLATINUM_OFFSETOF(LightComponent, direction))
				.WithField<Maths::Vec3>("ambientLight", PLATINUM_OFFSETOF(LightComponent, ambientLight))
				// don't serialize shaderInput!

				.WithField<float>("constant", PLATINUM_OFFSETOF(LightComponent, constant))
				.WithField<float>("linear", PLATINUM_OFFSETOF(LightComponent, linear))
				.WithField<float>("quadratic", PLATINUM_OFFSETOF(LightComponent, quadratic))
				.WithField<float>("intensity", PLATINUM_OFFSETOF(LightComponent, intensity));
	}

	LightComponent::LightComponent()
		:type(LightType::Point),
		 direction({0.0f, 1.0f, 0.0f}),
		 ambientLight({ 0.0f, 0.0f, 0.0f }),
		 intensity(1.0f),
		 spectrum(0.7f)
	{
	}
}

