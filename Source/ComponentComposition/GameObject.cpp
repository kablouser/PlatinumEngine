
#include <ComponentComposition/GameObject.h>
#include <SceneManager/Scene.h>
#include <ComponentComposition/Component.h>
#include <Helpers/VectorHelpers.h>
#include <Logger/Logger.h>

#include <utility>

namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	GameObject::GameObject() :
			_isEnabled(true),
			_isEnabledInHierarchy(true)
	{
	}

	GameObject::GameObject(
			std::string name,
			SavedReference<GameObject> parent,
			bool isEnabled) :
			name(std::move(name)),
			_parent(std::move(parent)),
			_isEnabled(isEnabled),
			_isEnabledInHierarchy(false)
	{
		_isEnabledInHierarchy = CalculateIsEnabledInHierarchy();
	}

	GameObject::~GameObject() = default;

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

	PlatinumEngine::SavedReference<GameObject>& GameObject::GetParent()
	{
		return _parent;
	}
  
	void GameObject::SetParent(SavedReference<GameObject> parent, Scene& scene)
	{
		if (_parent == parent)
			return;

		if (_parent)
		{
			_parent.pointer->RemoveChild(this);
		}
		else
		{
			// this has become NOT a root GameObject now
			scene.RemoveRootGameObject(*this);
		}
    
		if (parent)
		{
			std::shared_ptr<GameObject> sharedPointer = scene.idSystem.GetSavedReference(this);
			if (sharedPointer)
			{
				scene.idSystem.GetSavedReference<GameObject>(sharedPointer);
				parent.pointer->_children.push_back({sharedPointer})
			}
			else
			{

			}


			scene.idSystem.GetSavedReference<>()
			parent.pointer->_children.push_back(this);
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

	bool GameObject::MoveChildGameObjectPositionInList(GameObject* targetObject, GameObject* movedGameObject)
	{
		// get iterators for the selected target game object
		auto targetGameObjectIterator = std::find(_children.begin(), _children.end(), targetObject);

		// get iterator for the moved game object
		auto movedGameObjectIterator = std::find(_children.begin(), _children.end(), movedGameObject);

		// use rotate to move the item in front of the target object
		if(targetGameObjectIterator != _children.end() && movedGameObjectIterator != _children.end())
		{
			if(movedGameObjectIterator < targetGameObjectIterator)
				std::rotate(movedGameObjectIterator, movedGameObjectIterator+1, targetGameObjectIterator+1);

			else if (movedGameObjectIterator > targetGameObjectIterator)
				std::rotate(targetGameObjectIterator, movedGameObjectIterator, movedGameObjectIterator+1);

			// return true if moving successes
			return true;
		}

		// return false if moving fails
		return false;
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

	Component* GameObject::GetParentComponentInternal(const std::type_info& typeInfo)
	{
		GameObject* parent = GetParent();
		if (parent)
			return parent->GetComponentInternal(typeInfo);
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