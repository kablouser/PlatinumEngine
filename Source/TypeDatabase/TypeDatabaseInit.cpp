#include <TypeDatabase/TypeDatabase.h>
#include <IDSystem/IDSystem.h>
#include <SceneManager/Scene.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/RenderComponent.h>
#include <ComponentComposition/CameraComponent.h>

namespace PlatinumEngine
{
	void TypeDatabase::Init()
	{
		{
			auto idSystem = BeginTypeInfo<IDSystem>();
			auto& idSystemTypeInfo = idSystem.GetTypeInfo();
			idSystemTypeInfo.streamIn = IDSystem::StreamIn;
			idSystemTypeInfo.streamOut = IDSystem::StreamOut;
		}

		//------------------------------------------------------------------------------------------------------------------
		// Scene, GameObject and Components, needs access to privates to create type info
		//------------------------------------------------------------------------------------------------------------------

		Scene::CreateTypeInfo(*this);
		GameObject::CreateTypeInfo(*this);
		Component::CreateTypeInfo(*this);
		RenderComponent::CreateTypeInfo(*this);
		CameraComponent::CreateTypeInfo(*this);
		TransformComponent::CreateTypeInfo(*this);

		//------------------------------------------------------------------------------------------------------------------
		// Purely public objects
		//------------------------------------------------------------------------------------------------------------------

		Material::CreateTypeInfo(*this);

		//------------------------------------------------------------------------------------------------------------------
		// Plain-old-data (PODS)
		//------------------------------------------------------------------------------------------------------------------

		BeginTypeInfo<Color>()
				.WithField<float>("r", PLATINUM_OFFSETOF(Color, r))
				.WithField<float>("g", PLATINUM_OFFSETOF(Color, g))
				.WithField<float>("b", PLATINUM_OFFSETOF(Color, b))
				.WithField<float>("a", PLATINUM_OFFSETOF(Color, a));
		BeginTypeInfo<Maths::Vec3>()
				.WithField<float>("x", PLATINUM_OFFSETOF(Maths::Vec3, x))
				.WithField<float>("y", PLATINUM_OFFSETOF(Maths::Vec3, y))
				.WithField<float>("z", PLATINUM_OFFSETOF(Maths::Vec3, z));
		BeginTypeInfo<Maths::Quaternion>()
				.WithField<float>("x", PLATINUM_OFFSETOF(Maths::Quaternion, x))
				.WithField<float>("y", PLATINUM_OFFSETOF(Maths::Quaternion, y))
				.WithField<float>("z", PLATINUM_OFFSETOF(Maths::Quaternion, z))
				.WithField<float>("w", PLATINUM_OFFSETOF(Maths::Quaternion, w));
		BeginTypeInfo<Maths::Mat4>() // row major
				.WithField<float>("[0,0]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[0]))
				.WithField<float>("[0,1]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[1]))
				.WithField<float>("[0,2]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[2]))
				.WithField<float>("[0,3]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[3]))

				.WithField<float>("[1,0]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[4+0]))
				.WithField<float>("[1,1]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[4+1]))
				.WithField<float>("[1,2]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[4+2]))
				.WithField<float>("[1,3]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[4+3]))

				.WithField<float>("[2,0]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[8+0]))
				.WithField<float>("[2,1]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[8+1]))
				.WithField<float>("[2,2]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[8+2]))
				.WithField<float>("[2,3]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[8+3]))

				.WithField<float>("[3,0]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[12+0]))
				.WithField<float>("[3,1]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[12+1]))
				.WithField<float>("[3,2]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[12+2]))
				.WithField<float>("[3,3]", PLATINUM_OFFSETOF(Maths::Mat4, matrix[12+3]));

		//------------------------------------------------------------------------------------------------------------------
		// Asset data types. You don't need to describe their fields, just that they exist.
		//------------------------------------------------------------------------------------------------------------------

		BeginTypeInfo<Mesh>();
		BeginTypeInfo<Texture>();

		// this checks all fields/things are defined properly
		FinalCheck();
	}
}