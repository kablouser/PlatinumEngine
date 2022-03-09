#include "ComponentComposition/GameObject.h"

#include "iostream"
namespace PlatinumEngine
{
	GameObject::GameObject()
	{

	}

	GameObject::GameObject(std::string name): name(name)
	{
		GameObject();
	}

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

	int GameObject::GetComponentCount()
	{
		return _components.size();
	}
}