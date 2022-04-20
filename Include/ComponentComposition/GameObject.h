
#pragma once

#include <string>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include "ComponentComposition/Component.h"

namespace PlatinumEngine
{
	class Scene;

	class Component;

	class GameObject
	{
		friend class Scene;

		// only friends with base Component class, not its derivatives
		friend class Component;

	public:
    
		//--------------------------------------------------------------------------------------------------------------
		// _isEnabled control
		//--------------------------------------------------------------------------------------------------------------

		// Inconsiderate of hierarchy
		bool IsEnabled() const;

		void SetEnabled(bool isEnabled, Scene& scene);

		/**
		 * @return true iff this and all its' parents are enabled, false otherwise
		 */
		bool IsEnabledInHierarchy() const;

		//--------------------------------------------------------------------------------------------------------------
		// _parent control
		//--------------------------------------------------------------------------------------------------------------

		// Gets the parent of the current GameObject
		GameObject* GetParent();

		// Sets the parent of the current GameObject
		// Removes it from old parent, updates the parent and then add to new parent
		void SetParent(GameObject* parent, Scene& scene);
    
		//--------------------------------------------------------------------------------------------------------------
		// _children control
		// note: if you want to remove a child from this GameObject, use GameObject::SetParent(nullptr);
		//--------------------------------------------------------------------------------------------------------------

		// Returns children count
		size_t GetChildrenCount() const;

		// Returns child at index
		GameObject* GetChild(size_t index);

		// Returns index of child based on their name
		// (size_t)-1 if it doesn't exist
		size_t GetChildIndex(GameObject* child) const;

		// Returns boolean to check if moving successes or not
		// targetObject : a target object of which the moved object wants to move in front of
		// movedGameObject : an object that want to be moved
		bool MoveChildGameObjectPositionInList(GameObject* targetObject, GameObject* movedGameObject);

		//--------------------------------------------------------------------------------------------------------------
		// _components control
		//--------------------------------------------------------------------------------------------------------------

		// Current component count
		size_t GetComponentCount() const;

		Component* GetComponent(size_t index);

		// Returns existing component or nullptr if it doesn't exist
		template<class T>
		T* GetComponent()
		{
			return dynamic_cast<T*>(GetComponentInternal(typeid(T)));
		}

		//--------------------------------------------------------------------------------------------------------------
		// Convenience functions
		//--------------------------------------------------------------------------------------------------------------

		template<class T>
		T* GetParentComponent()
		{
			return dynamic_cast<T*>(GetParentComponentInternal(typeid(T)));
		}

		std::string name;

	private:

		//--------------------------------------------------------------------------------------------------------------
		// Constructors/destructors
		//--------------------------------------------------------------------------------------------------------------

		GameObject();

		explicit GameObject(std::string name = "GameObject", GameObject* parent = nullptr, bool isEnabled = true);

		~GameObject();

		// Cannot copy/move. Complex scene hierarchy structure cannot be determined from this object alone.
		// Use Scene to copy/move GameObjects
		GameObject(GameObject const&) = delete;

		GameObject& operator=(GameObject const&) = delete;

		GameObject(GameObject&&) noexcept = delete;

		GameObject& operator=(GameObject&&) noexcept = delete;

		GameObject* _parent;
		bool _isEnabled;
		bool _isEnabledInHierarchy;
		std::vector<GameObject*> _children;
		std::vector<Component*> _components;

		//--------------------------------------------------------------------------------------------------------------
		// Internal controls
		//--------------------------------------------------------------------------------------------------------------

		Component* GetComponentInternal(const std::type_info& typeInfo);

		Component* GetParentComponentInternal(const std::type_info& typeInfo);

		// note: if you want to remove child from this GameObject, use GameObject::SetParent(nullptr);
		void RemoveChild(GameObject* child);

		// note: if you want to remove component from this GameObject, use Component::SetGameObject(nullptr);
		void RemoveComponent(Component* component);

		// Calculates whether the current value of IsEnabledInHierarchy using this parent
		// relies on parent being in a valid state
		bool CalculateIsEnabledInHierarchy() const;

		// Must be called everytime IsEnabledInHierarchy might be changed
		// IsEnabledInHierarchy might change if _parent or _isEnabled is changed
		// Uses scene to broadcast events when there's a change
		void UpdateIsEnabledInHierarchy(Scene& scene);
	};

}

