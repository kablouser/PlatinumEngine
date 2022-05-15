#include <TypeDatabase/TypeDatabase.h>
#include <IDSystem/IDSystem.h>
#include <SceneManager/Scene.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/MeshRender.h>
#include <ComponentComposition/Camera.h>
#include <ComponentComposition/AnimationComponent.h>
#include <ComponentComposition/AnimationAttachment.h>
#include <Animation/AnimationLocalTimer.h>
#include <ComponentComposition/BoxCollider.h>
#include <ComponentComposition/SphereCollider.h>
#include <ComponentComposition/CapsuleCollider.h>
#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/AudioComponent.h>
#include <ComponentComposition/ParticleEffect.h>
#include <ParticleEffects/ParticleEmitter.h>
#include <ComponentComposition/Light.h>
#include <Audio/AudioClip.h>
#include <PlatformingGame/Player.h>
#include <PlatformingGame/DayNightCycle.h>

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
		MeshRender::CreateTypeInfo(*this);
		Camera::CreateTypeInfo(*this);
		Transform::CreateTypeInfo(*this);
		AnimationComponent::CreateTypeInfo(*this);
		AnimationAttachment::CreateTypeInfo(*this);
		AudioComponent::CreateTypeInfo(*this);
		RigidBody::CreateTypeInfo(*this);
		BoxCollider::CreateTypeInfo(*this);
		SphereCollider::CreateTypeInfo(*this);
		CapsuleCollider::CreateTypeInfo(*this);
		ParticleEffects::ParticleEmitter::CreateTypeInfo(*this);
		ParticleEffect::CreateTypeInfo(*this);
		AnimationLocalTimer::CreateTypeInfo(*this);
		LightComponent::CreateTypeInfo(*this);

		//------------------------------------------------------------------------------------------------------------------
		// Purely public objects
		//------------------------------------------------------------------------------------------------------------------

		Material::CreateTypeInfo(*this);
		BeginTypeInfo<PhysicsMaterial>()
		        .WithField<float>("friction", PLATINUM_OFFSETOF(PhysicsMaterial, friction))
				.WithField<float>("bounciness", PLATINUM_OFFSETOF(PhysicsMaterial, bounciness))
				.WithField<float>("damping", PLATINUM_OFFSETOF(PhysicsMaterial, damping))
				.WithField<float>("angularDamping", PLATINUM_OFFSETOF(PhysicsMaterial, angularDamping));

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
		BeginTypeInfo<Maths::Vec4>()
				.WithField<float>("x", PLATINUM_OFFSETOF(Maths::Vec4, x))
				.WithField<float>("y", PLATINUM_OFFSETOF(Maths::Vec4, y))
				.WithField<float>("z", PLATINUM_OFFSETOF(Maths::Vec4, z))
				.WithField<float>("w", PLATINUM_OFFSETOF(Maths::Vec4, w));
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
		BeginTypeInfo<Spectrum>()
		        .WithField<float>("r", PLATINUM_OFFSETOF(Spectrum, r))
				.WithField<float>("g", PLATINUM_OFFSETOF(Spectrum, g))
				.WithField<float>("b", PLATINUM_OFFSETOF(Spectrum, b));

		//------------------------------------------------------------------------------------------------------------------
		// Asset data types. You don't need to describe their fields, just that they exist.
		//------------------------------------------------------------------------------------------------------------------

		BeginTypeInfo<Mesh>();
		BeginTypeInfo<Texture>();
		BeginTypeInfo<AudioClip>();

		//------------------------------------------------------------------------------------------------------------------
		// Platforming game
		//------------------------------------------------------------------------------------------------------------------

		Player::CreateTypeInfo();
		DayNightCycle::CreateTypeInfo(*this);

		// this checks all fields/things are defined properly
		FinalCheck();
	}
}