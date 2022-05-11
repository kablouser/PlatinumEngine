#include <ComponentComposition/GameObject.h>
#include <SceneManager/Scene.h>
#include <Logger/Logger.h>
#include <algorithm>
#include <Application.h>


namespace PlatinumEngine
{
	void GameObject::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		/*
  		std::string name;
		SavedReference<GameObject> _parent;
		bool _isEnabled;
		bool _isEnabledInHierarchy;
		std::vector<SavedReference<GameObject>> _children;
		std::vector<SavedReference<Component>> _components;
		 */
		typeDatabase.BeginTypeInfo<GameObject>()
				.WithField<std::string>("name", PLATINUM_OFFSETOF(GameObject, name))
				.WithField<SavedReference<GameObject>>("_parent", PLATINUM_OFFSETOF(GameObject, _parent))
				.WithField<bool>("_isEnabled", PLATINUM_OFFSETOF(GameObject, _isEnabled))
				.WithField<bool>("_isEnabledInHierarchy", PLATINUM_OFFSETOF(GameObject, _isEnabledInHierarchy))
				.WithField<std::vector<SavedReference<GameObject>>>("_children", PLATINUM_OFFSETOF(GameObject, _children))
				.WithField<std::vector<SavedReference<Component>>>("_components", PLATINUM_OFFSETOF(GameObject, _components));
	}

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

	void GameObject::SetEnabled(bool isEnabled)
	{
		if (_isEnabled == isEnabled)
			return;

		_isEnabled = isEnabled;
		SavedReference<GameObject> referenceToThis = Application::Instance->idSystem.GetSavedReference(this);
		UpdateIsEnabledInHierarchy(referenceToThis);
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

	void GameObject::SetParent(SavedReference<GameObject> parent)
	{
		if (_parent == parent)
			return;

		SavedReference<GameObject> referenceToThis = Application::Instance->idSystem.GetSavedReference(this);
		if (!referenceToThis)
		{
			PLATINUM_WARNING("This GameObject is not in the ID System, cannot SetParent");
			return;
		}

		if (_parent)
			_parent.DeRef()->RemoveChild(referenceToThis);
		else
			// this has become NOT a root GameObject now
			Application::Instance->scene.RemoveRootGameObject(referenceToThis);

		if (parent)
			parent.DeRef()->_children.push_back(referenceToThis);
		else
			// this has become a root GameObject now
			Application::Instance->scene._rootGameObjects.push_back(referenceToThis);

		_parent = std::move(parent);
		UpdateIsEnabledInHierarchy(referenceToThis);
	}

	//--------------------------------------------------------------------------------------------------------------
	// _children control
	// note: if you want to remove a child from this GameObject, use GameObject::SetParent(nullptr);
	//--------------------------------------------------------------------------------------------------------------

	size_t GameObject::GetChildrenCount() const
	{
		return _children.size();
	}

	SavedReference<GameObject>& GameObject::GetChild(size_t index)
	{
		// range-checked indexing
		return _children.at(index);
	}

	size_t GameObject::GetChildIndex(GameObject* child) const
	{
		for (size_t i = 0; i < _children.size(); i++)
			if (_children[i].DeRef().get() == child)
				return i;
		return (size_t)-1;
	}

	bool GameObject::MoveChildGameObjectPositionInList(
			SavedReference<GameObject>& targetObject,
			SavedReference<GameObject>& movedGameObject)
	{
		// get iterators for the selected target game object
		auto targetGameObjectIterator = std::find(_children.begin(), _children.end(), targetObject);

		// get iterator for the moved game object
		auto movedGameObjectIterator = std::find(_children.begin(), _children.end(), movedGameObject);

		// use rotate to move the item in front of the target object
		if (targetGameObjectIterator != _children.end() && movedGameObjectIterator != _children.end())
		{
			if (movedGameObjectIterator < targetGameObjectIterator)
				std::rotate(movedGameObjectIterator, movedGameObjectIterator + 1, targetGameObjectIterator + 1);

			else if (movedGameObjectIterator > targetGameObjectIterator)
				std::rotate(targetGameObjectIterator, movedGameObjectIterator, movedGameObjectIterator + 1);

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

	SavedReference<Component>& GameObject::GetComponent(size_t index)
	{
		// range-checked indexing
		return _components.at(index);
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal controls
	//--------------------------------------------------------------------------------------------------------------

	void GameObject::RemoveChild(SavedReference<GameObject>& child)
	{
		for (size_t i = 0; i < _children.size(); ++i)
		{
			if (_children[i] == child)
			{
				_children.erase(_children.begin() + i);
				return;
			}
		}
		PLATINUM_ERROR("Hierarchy is invalid: _children is missing an element");
	}

	void GameObject::RemoveComponent(SavedReference<Component>& component)
	{
		for (size_t i = 0; i < _components.size(); ++i)
		{
			if (_components[i] == component)
			{
				_components.erase(_components.begin() + i);
				return;
			}
		}
		PLATINUM_ERROR("Hierarchy is invalid");
	}

	bool GameObject::CalculateIsEnabledInHierarchy() const
	{
		if (_parent)
			return _isEnabled && _parent.DeRef()->_isEnabledInHierarchy;
		else
			return _isEnabled;
	}

	void GameObject::UpdateIsEnabledInHierarchy(SavedReference<GameObject>& referenceToThis)
	{
		bool isEnabledInHierarchyNow = CalculateIsEnabledInHierarchy();

		if (_isEnabledInHierarchy == isEnabledInHierarchyNow)
			return;

		_isEnabledInHierarchy = isEnabledInHierarchyNow;

		Application::Instance->scene.UpdateIsEnabledInHierarchy(referenceToThis);
	}

	void GameObject::OnIDSystemUpdate()
	{
		_parent.OnIDSystemUpdate(Application::Instance->idSystem);
		for (auto& child : _children)
			child.OnIDSystemUpdate(Application::Instance->idSystem);
		for (auto& component : _components)
			component.OnIDSystemUpdate(Application::Instance->idSystem);
	}
}