#pragma once

#include <Application.h>


namespace PlatinumEngine
{
	template<typename T>
	SavedReference<T> Scene::AddComponent(SavedReference<GameObject> gameObject, bool isEnabled)
	{
		SavedReference<T> componentReference = Application::Instance->idSystem.Add<T>();
		AddComponentInternal(
				(SavedReference<Component>)componentReference,
				gameObject,
				isEnabled);
		return componentReference;
	}

	template<typename T>
	void Scene::RemoveComponent(SavedReference<T> component)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
		// we can use UnsafeCast because Component is base of T
		RemoveComponentInternal(std::move(component.template UnsafeCast<Component>()));
	}

	template<typename T>
	SavedReference<T> Scene::FindFirstComponent(bool requireEnabled)
	{
		// cannot use recursive functions because template T needed
		// for dynamic casting

		std::queue<SavedReference<GameObject>*> gameObjectsToFind;
		for (auto& root: _rootGameObjects)
			gameObjectsToFind.push(&root);

		while (!gameObjectsToFind.empty())
		{
			SavedReference<GameObject>* nextGameObject = gameObjectsToFind.front();
			gameObjectsToFind.pop();

			if (!nextGameObject->operator bool())
				continue;

			GameObject* rawPointer = nextGameObject->DeRef().get();
			if (requireEnabled && !rawPointer->_isEnabledInHierarchy)
				continue;

			// this is the dynamic cast
			SavedReference<T> targetComponent = rawPointer->GetComponent<T>();
			if (targetComponent)
			{
				if (!requireEnabled || targetComponent.DeRef()->_isEnabledInHierarchy)
					return targetComponent;
			}

			for (SavedReference<GameObject>& child: nextGameObject->DeRef()->_children)
				gameObjectsToFind.push(&child);
		}

		return {};
	}
}