#include "ComponentComposition/GameObject.h"
namespace PlatinumEngine
{
	GameObject::GameObject() : _hasComponent(false)
	{}

	GameObject::GameObject(std::string name): name(name), _hasComponent(false)
	{}

	//Gets whatever component and assign it. Also set flag to true
	void GameObject::AddComponent(Component component)
	{
		_component = component;
		_hasComponent = true;
	}

	//Will just return flag's state. Makes things easier
	bool GameObject::HasComponent()
	{
		return _hasComponent;
	}

	//Just returns whatever component is there
	//Will need to decide on Components to support different types
	Component GameObject::GetComponent()
	{
		return _component;
	}

	//Sets the pointer to NULL although setting flag to false should suffice
	void GameObject::RemoveComponent()
	{
		Component *ptr = &_component;
		ptr = NULL;
		_hasComponent = false;
	}

	// Will need to probably decide how different Components are created
	bool GameObject::HasLightComponent()
	{

	}

	bool GameObject::HasCameraComponent()
	{

	}
}