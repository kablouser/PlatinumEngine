//
// Created by Jinyuan on 04/04/2022.
//
#include "ComponentComposition/LightComponent.h"
#include <ComponentComposition/TransformComponent.h>
#include <ComponentComposition/Objects.h>
namespace PlatinumEngine
{
	LightComponent::LightComponent()
		:type(LightType::Directional),
		 direction({0.0f, 1.0f, 0.0f}),
		 ambientLight({ 0.2f, 0.2f, 0.2f }),
		 intensity(1.0f),
		 spectrum(1.0f)
	{
		if(type == LightType::Directional)
		{
			Mesh arrow = ArrowMesh(0.03f, 0.075f, 1.0f);
			shaderInput.Set(arrow.vertices, arrow.indices);
		}
		else if(type == LightType::Point)
		{
			Mesh arrow = SphereMesh(0.3f, 10);
			shaderInput.Set(arrow.vertices, arrow.indices);
		}
	}
	LightComponent::LightComponent(LightType type)
		:type(type),
		 direction({0.0f, 0.0f, -1.0f}),
		 ambientLight({ 0.2f, 0.2f, 0.2f }),
		 intensity(1.0f),
		 spectrum(1.0f)
	{
		if(type == LightType::Directional)
		{
			Mesh arrow = ArrowMesh(0.03f, 0.075f, 1.0f);
			shaderInput.Set(arrow.vertices, arrow.indices);
		}
		else if(type == LightType::Point)
		{
			Mesh arrow = SphereMesh(0.3f, 10);
			shaderInput.Set(arrow.vertices, arrow.indices);
		}
	}

	LightComponent::~LightComponent()
	{

	}

	void LightComponent::OnRender(Scene& scene, Renderer& renderer)
	{
		auto transform = GetComponent<TransformComponent>();
		renderer.DrawLight(transform->GetLocalToWorldMatrix());
		shaderInput.Draw();
	}
}

