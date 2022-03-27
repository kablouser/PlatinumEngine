//
// Created by Shihua on 09/03/2022.
//

#pragma once

// Platinum lib
#include <ComponentComposition/GameObject.h>
#include <Renderer/Renderer.h>

namespace PlatinumEngine
{

	class Scene
	{

	public:
		///___FUNCTIONS___

		GameObject* AddGameObject(GameObject* parent, std::string name);

		void RemoveGameObject(GameObject*);

		int GetRootGameObjectsCount();

		GameObject* GetRootGameObject(int index);

		template<typename T>
		Component* AddComponent()
		{
			Component* component = new T();

			_components.push_back(component);

			return component;
		}

		void RemoveComponent(Component*);

		void StartComponents(GameObject* gameObject);

		void EndComponents(GameObject* gameObject);

		void UpdateComponents(GameObject* gameObject, float deltaTime);

		void Start();

		void End();

		void Update(float deltaTime);

		void Render(Renderer* renderer);

		std::vector<GameObject*> _gameObjects;
		std::vector<GameObject*> _rootGameObjects;
		std::vector<Component*> _components;


		///___CONSTRUCTOR___
		Scene(){};

		///___DESTRUCTOR___
		//~Scene();


	};

}

