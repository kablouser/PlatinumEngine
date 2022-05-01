//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include <ComponentComposition/RenderComponent.h>
#include <Renderer/Renderer.h>
#include <ComponentComposition/TransformComponent.h>

namespace PlatinumEngine
{
	void RenderComponent::CreateTypeInfo(TypeDatabase& database)
	{
		/*
		Material material;
		SavedReference<Mesh> _mesh;
		ShaderInput _shaderInput;
		 */
		database.BeginTypeInfo<RenderComponent>()
		        .WithField<Material>("material", PLATINUM_OFFSETOF(RenderComponent, material))
				.WithField<SavedReference<Mesh>>("_mesh", PLATINUM_OFFSETOF(RenderComponent, _mesh));
	}

	RenderComponent::RenderComponent()
	{
	}

	void RenderComponent::OnRender(Scene& scene, Renderer& renderer)
	{
		if (!_mesh)
			return;

		SavedReference<TransformComponent> transform = GetComponent<TransformComponent>();
		if (transform)
			renderer.SetModelMatrix(transform.pointer->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();

		renderer.LoadMaterial(material);
		_shaderInput.Draw();
	}

	void RenderComponent::OnIDSystemUpdate(Scene& scene)
	{
		_shaderInput.Clear();
		if (_mesh)
			_shaderInput.Set(_mesh.pointer->vertices, _mesh.pointer->indices);
	}

	void RenderComponent::SetMesh(SavedReference<Mesh> mesh)
	{
		_mesh = std::move(mesh);
		_shaderInput.Clear();
		if (_mesh)
			_shaderInput.Set(_mesh.pointer->vertices, _mesh.pointer->indices);
	}

	void RenderComponent::SetMaterial(SavedReference<Texture> texture)
	{
		material.diffuseTexture = std::move(texture);
	}

	void RenderComponent::SetNormalMap(SavedReference<Texture> texture)
	{
		material.normalTexture = std::move(texture);
	}

	SavedReference<Mesh>& RenderComponent::GetMesh()
	{
		return _mesh;
	}
}
