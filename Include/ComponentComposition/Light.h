//
// Created by Jinyuan on 04/04/2022.
//
#pragma once
#include <ComponentComposition/Component.h>
#include <Maths/Vectors.h>
#include <Inspector/Spectrum.h>
#include <OpenGL/ShaderInput.h>

namespace PlatinumEngine
{
	class LightComponent : public Component
	{
	public:
		enum class LightType : int
		{
			Directional = 0,
			Point = 1,
			count
		};
		const char* LightTypeNames[(int)LightType::count] = {"Directional", "Point" };

		LightType type;
		Spectrum spectrum;
		Maths::Vec3 direction;
		Maths::Vec3 ambientLight;
		ShaderInput shaderInput;

		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;
		float intensity;

		LightComponent();
		~LightComponent();
		void UpdateMesh();
		explicit LightComponent(LightType type);

		virtual void OnRender() override;

	};
}
