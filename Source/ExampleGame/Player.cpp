#include <ExampleGame/Player.h>
#include <SceneManager/Scene.h>
#include <InputManager/InputManager.h>

namespace PlatinumEngine
{
	void Player::OnStart(Scene& scene)
	{
		_rigidBody = GetComponent<RigidBody>();
	}

	void Player::OnUpdate(Scene& scene, double deltaTime)
	{
	}

	void Player::OnIDSystemUpdate(Scene& scene)
	{
		_rigidBody.OnIDSystemUpdate(scene.idSystem);
	}
}