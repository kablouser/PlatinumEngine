//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include "ComponentComposition/RenderComponent.h"

namespace PlatinumEngine
{
	RenderComponent::RenderComponent()
	{
		SetMesh(nullptr);
	}

	void RenderComponent::OnRender(Scene& scene, Renderer &render)
	{
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

