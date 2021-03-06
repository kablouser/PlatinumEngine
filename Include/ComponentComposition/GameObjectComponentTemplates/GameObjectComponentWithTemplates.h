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
			SavedReference<T> castedReference = (SavedReference<T>)component;
			if (castedReference)
				return std::move(castedReference);
		}
		return {}; // nullptr
	}

	template<class T>
	SavedReference<T> GameObject::GetParentComponent()
	{
		SavedReference<GameObject>& parent = GetParent();
		if (parent)
			return parent.DeRef()->GetComponent<T>();
		return {}; // nullptr
	}

	template<class T>
	SavedReference<T> Component::GetComponent()
	{
		if (_gameObject)
			return _gameObject.DeRef()->GetComponent<T>();
		return {};
	}

	template<class T>
	SavedReference<T> Component::GetParentComponent()
	{
		if (_gameObject)
			return _gameObject.DeRef()->GetParentComponent<T>();
		return {};
	}
}