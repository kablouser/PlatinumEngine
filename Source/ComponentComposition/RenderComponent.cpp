//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include "ComponentComposition/RenderComponent.h"

namespace PlatinumEngine
{
	RenderComponent::RenderComponent()
	{
		LoadMesh("C:/Users/asus/Desktop/course/games101/Assignment3/models/rock/rock.obj");
	}
	RenderComponent::~RenderComponent() {}

	RenderComponent::RenderComponent(Mesh &mesh)
	{
		_mesh = mesh;
		_shaderInput.Set(_mesh.vertices, _mesh.indices);
		// _material = material;
	}

	void RenderComponent::OnRender(Scene& scene, Renderer &render)
	{
		_shaderInput.Draw();
	}

	void RenderComponent::LoadMesh(const std::string &filePath)
	{
		_mesh = Loaders::LoadMesh(filePath);
		_shaderInput.Set(_mesh.vertices, _mesh.indices);
	}

	Mesh& RenderComponent::GetMesh()
	{
		return _mesh;
	}
}

