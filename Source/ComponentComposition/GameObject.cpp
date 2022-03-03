#include "ComponentComposition/GameObject.h"
namespace PlatinumEngine
{
	GameObject::GameObject() : _hasComponent(false)
	{}

	GameObject::GameObject(std::string name): name(name), _hasComponent(false)
	{}

	void GameObject::AddComponent(Component component)
	{
		_component = component;
		_hasComponent = true;
	}

	bool GameObject::HasComponent()
	{
		return _hasComponent;
	}

	Component GameObject::GetComponent()
	{
		return _component;
	}

	void GameObject::RemoveComponent()
	{
		Component *ptr = &_component;
		ptr = NULL;
		_hasComponent = false;
	}

	bool GameObject::HasLightComponent()
	{

	}

	bool GameObject::HasCameraComponent()
	{

	}
}