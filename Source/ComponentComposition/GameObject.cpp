#include "ComponentComposition/GameObject.h"
namespace PlatinumEngine
{
	GameObject::GameObject()
	{}

	GameObject::GameObject(std::string name): name(name)
	{
		GameObject();
	}
	GameObject::~GameObject()
	{
		_components.clear();
		for(auto* child:_children)
			child->SetParent(NULL);
		_children.clear();
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

	//Gets the parent of the current GameObject
	GameObject* GameObject::GetParent()
	{
		return _parent;
	}

	//Sets the parent of the current GameObject
	//Essentially can be used to remove a child object also
	void GameObject::SetParent(GameObject* parent)
	{
		_parent = parent;
	}

	//Returns children count
	int GameObject::GetChildrenCount()
	{
		return _children.size();
	}

	//Returns child at index
	GameObject* GameObject::GetChild(int index)
	{
		return _children[index];
	}

	//Removes component at index
	void GameObject::RemoveComponent(int index)
	{
		_components.erase(_components.begin()+index);
	}

	//Current component count
	int GameObject::GetComponentCount()
	{
		return _components.size();
	}
}