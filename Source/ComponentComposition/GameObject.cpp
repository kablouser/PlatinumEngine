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

	//Enable/Disable the GameObject
	void GameObject::SetEnabled(bool enableFlag)
	{
		_isEnabled = enableFlag;
	}

	//Gets the parent of the current GameObject
	GameObject* GameObject::GetParent()
	{
		return _parent;
	}

	//Sets the parent of the current GameObject
	//Removes it from old parent, updates the parent and then add to new parent
	void GameObject::SetParent(GameObject* parent)
	{
		_parent->RemoveChild(this->name);
		_parent = parent;
		_parent->_children.push_back(this);
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

	//Returns index of child based on their name
	//-1 if it doesn't exist
	int GameObject::GetChildIndex(std::string childName)
	{
		for(int i=0;i<_children.size();i++)
			if(_children[i]->name==childName)
				return i;
		return -1;
	}

	//Deletes child on the basis of their name
	void GameObject::RemoveChild(std::string childName)
	{
		int index = GetChildIndex(childName);
		if(index>=0)
			_children.erase(_children.begin()+index);
	}
}