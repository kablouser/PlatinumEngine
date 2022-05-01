//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include "ComponentComposition/MeshRender.h"
#include <Renderer/Renderer.h>
#include <ComponentComposition/Transform.h>

namespace PlatinumEngine
{

	MeshRender::MeshRender()
	{
		SetMesh(nullptr);
		SetMaterial(nullptr);
	}

	void MeshRender::OnRender(Scene& scene, Renderer& renderer)
	{

		Transform* transform = GetComponent<Transform>();

		if (transform)
			renderer.SetModelMatrix(transform->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();


		// set animation matrix
		if(_mesh!= nullptr)
		{
			renderer.SetAnimationStatus(isAnimationDisplay);

			if (!_animations.empty())
			{
				_animations[selectedAnimationIndex]->UpdateWorldTransformMatrix(_mesh->skeleton, _mesh->bones);
				_animations[selectedAnimationIndex]->UpdateAnimationTime();

				for (unsigned int i = 0; i < _animations[selectedAnimationIndex]->worldTransform.size(); ++i)
				{
					renderer.SetAnimationTransform(i, _animations[selectedAnimationIndex]->worldTransform[i]);
				}
			}
		}

		// load texture
		renderer.LoadTexture(material);
		_shaderInput.Draw();
	}

	void MeshRender::SetMaterial(Texture* texture)
	{
		material.diffuseTexture = texture;
	}

	void MeshRender::SetNormalMap(Texture* texture)
	{
		material.normalTexture = texture;
	}

	void MeshRender::SetMesh(Mesh* mesh)
	{
		_mesh = mesh;
		if(mesh != nullptr)
		{
			if(_mesh->isAnimationExist)
			{
				_shaderInput.Clear();
				_shaderInput.Set(_mesh->animationVertices, _mesh->indices);
			}
			else
			{
				_shaderInput.Clear();
				_shaderInput.Set(mesh->vertices, mesh->indices);
			}
		}
		else
		{
			_shaderInput.Clear();
		}
	}

	void RenderComponent::AddAnimation(Animation* animation)
	{
		_animations.push_back(animation);
	}

	Mesh* RenderComponent::GetMesh()
	{
		return _mesh;
	}

	std::vector<Animation*>& RenderComponent::GetAnimation()
	{
		return _animations;
	}
}
