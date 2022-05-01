//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include "ComponentComposition/MeshRender.h"
#include <Renderer/Renderer.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/AnimationComponent.h>
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
		AnimationComponent* animation = GetComponent<AnimationComponent>();

		if(animation!= nullptr && _mesh!= nullptr)
		{

			// send flag to uniform
			renderer.SetAnimationStatus(animation->isAnimationDisplay);

			// get animation list from animation component
			std::vector<Animation*> animationList = animation->GetAnimation();

			// calculate the transform matrices for a post at a specific animation time
			if (!animationList.empty())
			{
				animationList[animation->selectedAnimationIndex]->UpdateWorldTransformMatrix(_mesh->skeleton, _mesh->bones);
				animationList[animation->selectedAnimationIndex]->PlayAnimationTimer();

				// pass transform matrices to
				for (unsigned int i = 0; i < animationList[animation->selectedAnimationIndex]->worldTransform.size(); ++i)
				{
					renderer.SetAnimationTransform(i, animationList[animation->selectedAnimationIndex]->worldTransform[i]);
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


	Mesh* RenderComponent::GetMesh()
	{
		return _mesh;
	}

}
