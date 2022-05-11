//
// Created by Jinyuan on 04/04/2022.
//
#include "ComponentComposition/Light.h"
#include <ComponentComposition/TransformComponent.h>
#include <ComponentComposition/Objects.h>

namespace PlatinumEngine
{
	LightComponent::LightComponent()
		:type(LightType::Point),
		 direction({0.0f, 1.0f, 0.0f}),
		 ambientLight({ 0.2f, 0.2f, 0.2f }),
		 intensity(1.0f),
		 spectrum(0.7f)
	{
		UpdateMesh();
	}
	LightComponent::LightComponent(LightType type)
		:type(type),
		 direction({0.0f, 0.0f, -1.0f}),
		 ambientLight({ 0.2f, 0.2f, 0.2f }),
		 intensity(1.0f),
		 spectrum(1.0f)
	{
		UpdateMesh();
	}

	LightComponent::~LightComponent()
	{

	}

	void LightComponent::UpdateMesh()
	{
		if(type == LightType::Directional)
		{
			Mesh arrow = ArrowMesh(0.03f, 0.075f, 1.0f);
			shaderInput.Set(arrow.vertices, arrow.indices);
		}
		else if(type == LightType::Point)
		{
			Mesh sphere = SphereMesh(0.1f, 3);
			shaderInput.Set(sphere.vertices, sphere.indices);
		}
	}


	void LightComponent::OnRender(Scene& scene, Renderer& renderer)
	{
		auto transform = GetComponent<TransformComponent>();

		if(transform)
		{
			if(type == LightType::Point)
				transform->localScale = 0.1f;
			renderer.DrawLight(transform->GetLocalToWorldMatrix());
		}
		else
		{
			Maths::Mat4 matrix(1.f);
			if(type == LightType::Point)
				matrix.SetScaleMatrix(Maths::Vec3(0.1f, 0.1f, 0.1f));
			renderer.DrawLight(matrix);
		}
		shaderInput.Draw();
	}
}

