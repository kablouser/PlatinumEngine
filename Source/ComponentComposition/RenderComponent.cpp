//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include "ComponentComposition/RenderComponent.h"
#include <Renderer/Renderer.h>
#include <ComponentComposition/TransformComponent.h>

namespace PlatinumEngine
{
	RenderComponent::RenderComponent()
	{
		SetMesh(nullptr);
	}

	void RenderComponent::OnRender(Scene& scene, Renderer& renderer)
	{
		TransformComponent* transform = GetComponent<TransformComponent>();
		if (transform)
			renderer.SetModelMatrix(transform->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();

		_shaderInput.Draw();
	}

	void RenderComponent::SetMesh(Mesh* mesh)
	{
		_mesh = mesh;
		if(mesh != nullptr)
			_shaderInput.Set(mesh->vertices, mesh->indices);
		else
			_shaderInput.Clear();
	}

	Mesh* RenderComponent::GetMesh()
	{
		return _mesh;
	}

}

