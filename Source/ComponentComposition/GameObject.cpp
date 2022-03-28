
#include <ComponentComposition/GameObject.h>
#include <SceneManager/Scene.h>
#include <ComponentComposition/Component.h>
#include <Helpers/VectorHelpers.h>
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	GameObject::GameObject() :
			_parent(nullptr),
			_isEnabled(true),
			_isEnabledInHierarchy(true)
	{
	}

	GameObject::GameObject(std::string name, GameObject* parent, bool isEnabled) :
			name(std::move(name)),
			_parent(parent),
			_isEnabled(isEnabled),
			_isEnabledInHierarchy(false)
	{
		_isEnabledInHierarchy = CalculateIsEnabledInHierarchy();
	}

	GameObject::~GameObject()
	{
	}

	//--------------------------------------------------------------------------------------------------------------
	// _isEnabled control
	//--------------------------------------------------------------------------------------------------------------

	bool GameObject::IsEnabled() const
	{
		return _isEnabled;
	}
  
	void GameObject::SetEnabled(bool isEnabled, Scene& scene)
	{
		if (_isEnabled == isEnabled)
			return;

		_isEnabled = isEnabled;
		UpdateIsEnabledInHierarchy(scene);
	}

	bool GameObject::IsEnabledInHierarchy() const
	{
		return _isEnabledInHierarchy;
	}

	//--------------------------------------------------------------------------------------------------------------
	// _parent control
	//--------------------------------------------------------------------------------------------------------------

	GameObject* GameObject::GetParent()
	{
		return _parent;
	}
  
	void GameObject::SetParent(GameObject* parent, Scene& scene)
	{
		if (_parent == parent)
			return;

		if (_parent)
		{
			_parent->RemoveChild(this);
		}
		else
		{
			// this has become NOT a root GameObject now
			scene.RemoveRootGameObject(*this);
		}
    
		if (parent)
		{
			parent->_children.push_back(this);
		}
		else
		{
			// this has become a root GameObject now
			scene._rootGameObjects.push_back(this);
		}

		_parent = parent;
		UpdateIsEnabledInHierarchy(scene);
	}

	//--------------------------------------------------------------------------------------------------------------
	// _children control
	// note: if you want to remove a child from this GameObject, use GameObject::SetParent(nullptr);
	//--------------------------------------------------------------------------------------------------------------

	size_t GameObject::GetChildrenCount() const
	{
		return _children.size();
	}
  
	GameObject* GameObject::GetChild(size_t index)
	{
		// range-checked indexing
		return _children.at(index);
	}
  
	size_t GameObject::GetChildIndex(GameObject* child) const
	{
		for (size_t i = 0; i < _children.size(); i++)
			if (_children[i] == child)
				return i;
		return (size_t)-1;
	}
  
	//--------------------------------------------------------------------------------------------------------------
	// _components control
	//--------------------------------------------------------------------------------------------------------------

	size_t GameObject::GetComponentCount() const
	{
		return _components.size();
	}
  
	Component* GameObject::GetComponent(size_t index)
	{
		// range-checked indexing
		return _components.at(index);
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal controls
	//--------------------------------------------------------------------------------------------------------------

	Component* GameObject::GetComponentInternal(const std::type_info& typeInfo)
	{
		for (auto component: _components)
			if (typeid(*component) == typeInfo)
				return component;
		return nullptr;
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		if (!VectorHelpers::RemoveFirst(_children, child))
		{
			PLATINUM_ERROR("Hierarchy is invalid: _children is missing an element");
		}
	}

	void GameObject::RemoveComponent(Component* component)
	{
		if (!VectorHelpers::RemoveFirst(_components, component))
		{
			PLATINUM_ERROR("Hierarchy is invalid");
		}
	}

	bool GameObject::CalculateIsEnabledInHierarchy() const
	{
		if (_parent)
			return _isEnabled && _parent->_isEnabledInHierarchy;
		else
			return _isEnabled;
	}

	void GameObject::UpdateIsEnabledInHierarchy(Scene& scene)
	{
		bool isEnabledInHierarchyNow = CalculateIsEnabledInHierarchy();

		if (_isEnabledInHierarchy == isEnabledInHierarchyNow)
			return;

		_isEnabledInHierarchy = isEnabledInHierarchyNow;

		scene.UpdateIsEnabledInHierarchy(*this);
	}
}