#pragma once

// This weird structure avoids infinite loops in header files

#include <ComponentComposition/GameObjectComponentTemplates/GameObjectWithoutTemplates.h>
#include <ComponentComposition/GameObjectComponentTemplates/ComponentWithoutTemplates.h>

// templates!
namespace PlatinumEngine
{
	template<class T>
	SavedReference<T> GameObject::GetComponent()
	{
		for (SavedReference<Component>& component: _components)
		{
			std::shared_ptr<T> castedPointer = std::dynamic_pointer_cast<T>(component.pointer);
			if (castedPointer)
				return { component.id, castedPointer };
		}
		return {}; // nullptr
	}

	template<class T>
	SavedReference<T> GameObject::GetParentComponent()
	{
		SavedReference<GameObject>& parent = GetParent();
		if (parent)
			return parent.pointer->GetComponent<T>();
		return {}; // nullptr
	}

	template<class T>
	SavedReference<T> Component::GetComponent()
	{
		if (_gameObject)
			return _gameObject.pointer->GetComponent<T>();
		return {};
	}

	template<class T>
	SavedReference<T> Component::GetParentComponent()
	{
		if (_gameObject)
			return _gameObject.pointer->GetParentComponent<T>();
		return {};
	}
}