//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include "ComponentComposition/RenderComponent.h"

using namespace PlatinumEngine;
RenderComponent::RenderComponent() {}
RenderComponent::~RenderComponent() {}

RenderComponent::RenderComponent(const Mesh &mesh)
{
	_mesh = mesh;
	// _material = material;
}

void RenderComponent::OnRender(Renderer &render)
{
	render.LoadMesh(_mesh);
}

void RenderComponent::LoadMesh(const std::string &filePath)
{
	_mesh = Loaders::LoadMesh(filePath);
}

Mesh RenderComponent::GetMesh()
{
	return _mesh;
}
