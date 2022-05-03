//
// Created by Jinyuan on 04/04/2022.
//
#pragma once
#include <vector>
#include <Maths/Vectors.h>
#include "Renderer/Renderer.h"
#include "SceneManager/Scene.h"
#include "ComponentComposition/Component.h"

namespace PlatinumEngine
{
	enum LightType
	{
		DirectedLight = 0,
		PointLight = 1,
		SpotLight = 2
	};
	struct Light
	{
		Maths::Vec3 _direction;
		Maths::Vec3 _ambientLight;
		Maths::Vec3 _diffuseLight;
		Maths::Vec3 _specularLight;
		float _constant;
		float _linear;
		float _quadratic;

		float _cutOff;
		float _outerCutOff;
	};

	// TODO
	class LightComponent: public Component
	{
	public:
		LightComponent();
		~LightComponent();

		//--------------------------------------------------------------------------------------------------------------
		// Events
		//--------------------------------------------------------------------------------------------------------------
//		void OnStart() override;
//		void OnEnd() override;
//		void OnEnable(Scene& scene) override;
//		void OnDisable(Scene& scene) override;
//		void OnUpdate(Scene& scene, double deltaTime) override;
//		void OnRender(Scene& scene, Renderer& renderer) override;

		//--------------------------------------------------------------------------------------------------------------
		// Functions
		//--------------------------------------------------------------------------------------------------------------

	private:
		LightType _lightType;

	};
}
