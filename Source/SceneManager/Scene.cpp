//
// Created by Shihua on 09/03/2022.
//

#include <SceneManager/Scene.h>


namespace PlatinumEngine
{

	GameObject* Scene::AddGameObject(GameObject* parent, std::string name)
	{

		GameObject* gameObject = new GameObject(name);

		if (parent == nullptr)
		{
			_rootGameObjects.push_back(gameObject);
		}
		else
		{
			gameObject->SetParent(parent);

		}

		_gameObjects.push_back(gameObject);

		return gameObject;

	}


	void Scene::RemoveGameObject(GameObject* gameObject)
	{

		for (int i = 0; i < gameObject->GetChildrenCount(); i++)
		{
			RemoveGameObject(gameObject->GetChild(i));
		}

		// get parent
		GameObject* parent = gameObject->GetParent();
		if (parent != nullptr)
		{
			std::remove(parent->_children.begin(), parent->_children.end(), gameObject);
		}

		delete gameObject;

		std::remove(_gameObjects.begin(), _gameObjects.end(), gameObject);

	}

	int Scene::GetRootGameObjectsCount()
	{

		return _rootGameObjects.size();

	}


	GameObject* Scene::GetRootGameObject(int index)
	{

		return _rootGameObjects[index];

	}

	void Scene::RemoveComponent(Component* component)
	{

		delete component;

		std::remove(_components.begin(), _components.end(), component);

	}


	void Scene::Start()
	{

		for(auto iterator : _rootGameObjects)
		{

			if(iterator->IsEnabled())
			{

				StartComponents(iterator);

			}

		}

	}

	void Scene::End()
	{
		for(auto iterator : _rootGameObjects)
		{

			if(!iterator->IsEnabled())
			{

				StopComponents(iterator);

			}

		}

	}


	void Scene::StartComponents(GameObject* gameObject)
	{

		for(auto iterator:gameObject->_components)
		{
			if(iterator->IsEnabled())
				iterator->OnStart();
		}


		for(int i =0; i < gameObject->GetChildrenCount();i++)
		{
			GameObject* child = gameObject->GetChild(i);

			if(child->IsEnabled())
				StartComponents(child);
		}

	}

	void Scene::StopComponents(GameObject* gameObject)
	{

		for(auto iterator:gameObject->_components)
		{
			if(!iterator->IsEnabled())
				iterator->OnEnd();
		}


		for(int i =0; i < gameObject->GetChildrenCount();i++)
		{
			GameObject* child = gameObject->GetChild(i);

			if(!child->IsEnabled())
				StopComponents(child);
		}

	}

}