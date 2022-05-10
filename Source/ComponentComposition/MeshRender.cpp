//
// Created by Jinyuan and Shawn on 26/03/2022.
//
#include <ComponentComposition/MeshRender.h>
#include <Renderer/Renderer.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/AnimationComponent.h>
#include <SceneManager/Scene.h>
#include <Application.h>

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

	void MeshRender::OnRender()
	{
		if (!_mesh)
			return;

		SavedReference<Transform> transform = GetComponent<Transform>();
		if (transform)
			Application::Instance->renderer.SetModelMatrix(transform.DeRef()->GetLocalToWorldMatrix());
		else
			Application::Instance->renderer.SetModelMatrix();

		SavedReference<AnimationComponent> animation = GetComponent<AnimationComponent>();

		// set animation matrix
		if (animation)
		{

			// send flag to uniform
			Application::Instance->renderer.SetAnimationStatus(animation.DeRef()->GetIsDisplay());


			// calculate the transform matrices for a post at a specific animation time
			if (animation.DeRef()->GetAmountOfAnimations()!=0)
			{
				animation.DeRef()->UpdateWorldTransformMatrix(_mesh.DeRef()->skeleton,
						_mesh.DeRef()->bones, Application::Instance->time);

				// pass transform matrices to
				for (unsigned int i = 0; i < animation.DeRef()->worldTransform.size(); ++i)
				{
					Application::Instance->renderer.SetAnimationTransform(i, animation.DeRef()->worldTransform[i]);
				}
			}
		}
		else
			// send flag to uniform
			Application::Instance->renderer.SetAnimationStatus(false);

		// load texture
		Application::Instance->renderer.LoadMaterial(material);
		_shaderInput.Draw();
	}

	void MeshRender::OnIDSystemUpdate()
	{
		_mesh.OnIDSystemUpdate(Application::Instance->idSystem);
		material.OnIDSystemUpdate(Application::Instance->idSystem);
		_shaderInput.Clear();
		if (_mesh)
		{
			if(_mesh.DeRef()->animations.empty())
				_shaderInput.Set(_mesh.DeRef()->vertices, _mesh.DeRef()->indices);
			else
				_shaderInput.Set(_mesh.DeRef()->animationVertices, _mesh.DeRef()->indices);
		}
	}

	void MeshRender::SetMesh(SavedReference<Mesh> mesh)
	{
		_mesh = std::move(mesh);
		_shaderInput.Clear();
		if (_mesh)
		{
			if(_mesh.DeRef()->animations.empty())
				_shaderInput.Set(_mesh.DeRef()->vertices, _mesh.DeRef()->indices);
			else
				_shaderInput.Set(_mesh.DeRef()->animationVertices, _mesh.DeRef()->indices);
		}
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
