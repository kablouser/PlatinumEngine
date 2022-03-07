#include "ComponentComposition/GameObject.h"
namespace PlatinumEngine
{
	GameObject::GameObject()
	{}

	GameObject::GameObject(std::string name): name(name)
	{}

	bool GameObject::IsEnabled()
	{
		return _isEnabled;
	}

	//Enable/Disable child objects as well
	void GameObject::SetEnabled(bool enableFlag)
	{
		_isEnabled = enableFlag;
		for(GameObject* child:_children)
			child->SetEnabled(enableFlag);
	}

	GameObject* GameObject::GetParent()
	{
		return _parent;
	}

	void GameObject::SetParent(GameObject* parent)
	{
		_parent = parent;
	}

	int GameObject::GetChildrenCount()
	{
		return _children.size();
	}

	GameObject* GameObject::GetChild(int index)
	{
		return _children[index];
	}

	//Just push on the new component
	//Although, may need to do check to prevent same component being added
	void GameObject::AddComponent(Component* component)
	{
		_components.push_back(component);
	}

	//Searches for component based on name or type
	bool GameObject::HasComponent()
	{
		return !_components.empty();
	}

	//Will need to decide on Components to support different types
	//Will search for component based on name or type.
	Component* GameObject::GetComponent(int index)
	{
		return _components[index];
	}

	//Need to iterate and remove from Components vector
	void GameObject::RemoveComponent(int index)
	{
		_components.erase(_components.begin()+index);
	}

	// Will need to probably decide how different Components are created
	bool GameObject::HasLightComponent()
	{

	}

	bool GameObject::HasCameraComponent()
	{

	}
}