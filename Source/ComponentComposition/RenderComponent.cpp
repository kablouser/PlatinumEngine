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
		SetMaterial(nullptr);
	}

	void RenderComponent::OnRender(Scene& scene, Renderer& renderer)
	{
		SavedReference<TransformComponent> transform = GetComponent<TransformComponent>();
		if (transform)
			renderer.SetModelMatrix(transform.pointer->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();

		renderer.LoadMaterial(material);
		_shaderInput.Draw();
	}

	void RenderComponent::SetMaterial(Texture* texture)
	{
		material.diffuseTexture = texture;
	}

	void RenderComponent::SetNormalMap(Texture* texture)
	{
		material.normalTexture = texture;
	}

	void RenderComponent::SetMesh(Mesh* mesh)
	{
		_mesh = mesh;
		if(mesh != nullptr)
		{
			_shaderInput.Clear();
			_shaderInput.Set(mesh->vertices, mesh->indices);
		}
		else
			_shaderInput.Clear();
	}

	Mesh* RenderComponent::GetMesh()
	{
		return _mesh;
	}
}
