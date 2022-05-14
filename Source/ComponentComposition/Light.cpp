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
		UpdateMesh();
	}
	LightComponent::LightComponent(LightType type)
		:type(type),
		 direction({0.0f, 0.0f, -1.0f}),
		 ambientLight({ 0.0f, 0.0f, 0.0f }),
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
			auto transform = GetComponent<Transform>();
			if (transform)
				transform.DeRef()->localScale = 1.0f;
			shaderInput.Clear();
			shaderInput.Set(arrow.vertices, arrow.indices);
		}
		else if(type == LightType::Point)
		{
			Mesh sphere = SphereMesh(0.1f, 3);
			auto transform = GetComponent<Transform>();
			if (transform)
				transform.DeRef()->localScale = 0.01f;
			shaderInput.Clear();
			shaderInput.Set(sphere.vertices, sphere.indices);
		}
	}


	void LightComponent::OnRender()
	{
		auto transform = GetComponent<Transform>();

		if(transform)
		{
			if(type == LightType::Point)
				transform.DeRef()->localScale = 0.1f;
			Application::Instance->renderer.DrawLight(transform.DeRef()->GetLocalToWorldMatrix());
		}
		else
		{
			Maths::Mat4 matrix(1.f);
			if(type == LightType::Point)
				matrix.SetScaleMatrix(Maths::Vec3(0.1f, 0.1f, 0.1f));
			Application::Instance->renderer.DrawLight(matrix);
		}
		shaderInput.Draw();
	}
}

