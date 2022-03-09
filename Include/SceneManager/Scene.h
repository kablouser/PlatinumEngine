//
// Created by LEGION on 09/03/2022.
//

#pragma once


#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/Component.h>
#include <RasterRenderer/RasterRenderer.h>

namespace PlatinumEngine
{

	class Scene
	{

	public:

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

		void Start();

		void StopComponents(GameObject* gameObject);

		void End();

		void Update(float deltaTime);

		void Render(RasterRenderer* renderer);

		std::vector<GameObject*> _gameObjects;
		std::vector<GameObject*> _rootGameObjects;
		std::vector<Component*> _components;


		///___CONSTRUCTOR___
		Scene();

		///___DESTRUCTOR___
		~Scene();


	};

}

