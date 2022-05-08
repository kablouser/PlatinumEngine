#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/RigidBody.h>
#include <IDSystem/IDSystem.h>

namespace PlatinumEngine
{
	class Player : public Component
	{
	public:
		void OnStart(Scene& scene);
		void OnUpdate(Scene& scene, double deltaTime) override;
		void OnIDSystemUpdate(Scene& scene) override;

	private:
		SavedReference<RigidBody> _rigidBody;
	};
}