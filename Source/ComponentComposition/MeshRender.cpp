//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include <ComponentComposition/MeshRender.h>
#include <Renderer/Renderer.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/AnimationComponent.h>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	void MeshRender::CreateTypeInfo(TypeDatabase& database)
	{
		/*
		Material material;
		SavedReference<Mesh> _mesh;
		ShaderInput _shaderInput;
		 */
		database.BeginTypeInfo<MeshRender>()
				.WithInherit<Component>()
		        .WithField<Material>("material", PLATINUM_OFFSETOF(MeshRender, material))
				.WithField<SavedReference<Mesh>>("_mesh", PLATINUM_OFFSETOF(MeshRender, _mesh));
	}

	MeshRender::MeshRender()
	{
	}

	void MeshRender::OnRender(Scene& scene, Renderer& renderer)
	{
		if (!_mesh)
			return;

		SavedReference<Transform> transform = GetComponent<Transform>();
		if (transform)
			renderer.SetModelMatrix(transform.DeRef()->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();

		SavedReference<AnimationComponent> animation = GetComponent<AnimationComponent>();

		// set animation matrix
		if (animation)
		{
			AnimationComponent* animationComponent = animation.DeRef().get();

			// send flag to uniform
			renderer.SetAnimationStatus(animationComponent->isAnimationDisplay);

			// get animation list from animation component
			std::vector<Animation*> animationList = animationComponent->GetAnimation();

			// calculate the transform matrices for a post at a specific animation time
			if (!animationList.empty())
			{
				animationList[animationComponent->selectedAnimationIndex]->UpdateWorldTransformMatrix(
						_mesh.DeRef()->skeleton,
						_mesh.DeRef()->bones,
						animationComponent->context);
				animationList[animationComponent->selectedAnimationIndex]->PlayAnimationTimer();

				// pass transform matrices to
				for (unsigned int i = 0; i < animationList[animationComponent->selectedAnimationIndex]->worldTransform.size(); ++i)
				{
					renderer.SetAnimationTransform(i, animationList[animationComponent->selectedAnimationIndex]->worldTransform[i]);
				}
			}
		}

		renderer.LoadMaterial(material);
		_shaderInput.Draw();

		// load texture
		renderer.LoadMaterial(material);
		_shaderInput.Draw();
	}

	void MeshRender::OnIDSystemUpdate(Scene& scene)
	{
		_mesh.OnIDSystemUpdate(scene.idSystem);
		material.OnIDSystemUpdate(scene.idSystem);
		_shaderInput.Clear();
		if (_mesh)
			_shaderInput.Set(_mesh.DeRef()->vertices, _mesh.DeRef()->indices);
	}

	void MeshRender::SetMesh(SavedReference<Mesh> mesh)
	{
		_mesh = std::move(mesh);
		_shaderInput.Clear();
		if (_mesh)
			_shaderInput.Set(_mesh.DeRef()->vertices, _mesh.DeRef()->indices);
	}

	void MeshRender::SetMaterial(SavedReference<Texture> texture)
	{
		material.diffuseTexture = std::move(texture);
	}

	void MeshRender::SetNormalMap(SavedReference<Texture> texture)
	{
		material.normalTexture = std::move(texture);
	}

	SavedReference<Mesh>& MeshRender::GetMesh()
	{
		return _mesh;
	}
}
