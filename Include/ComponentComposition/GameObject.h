
#pragma once

#include <string>
#include <vector>
#include <typeinfo>
#include "ComponentComposition/Component.h"

#include "iostream"

namespace PlatinumEngine
{
	class GameObject
	{
	public:
		friend class Scene;
		std::string name;
	public:
		GameObject();
		GameObject(std::string name);

		~GameObject();

		bool IsEnabled();
		void SetEnabled(bool enableFlag);

		GameObject* GetParent();
		void SetParent(GameObject* parent);

		int GetChildrenCount();
		GameObject* GetChild(int index);

		void RemoveComponent(int index);
		int GetComponentCount();

		int GetChildIndex(GameObject* child);
		void RemoveChild(GameObject* child);

		//Adds a component
		template <class T> void AddComponent (T *component)
		{
			if(!HasComponent<T>())
				_components.emplace_back(component);
		}

		//Checks if GameObject has the component
		template<class T> bool HasComponent()
		{
			for(auto& c:_components)
				if(typeid(*c)==typeid(T))
					return true;
			return false;
		}

		//Returns existing component or nullptr if it doesn't exist
		template<class T> T* GetComponent()
		{
			for(auto& c:_components)
				if(typeid(*c)==typeid(T))
					return dynamic_cast<T*>(c);
			return nullptr;
		}

		//Removes component
		template<class T> void RemoveComponent()
		{
			int index=-1, counter=0;
			for(auto& c:_components)
			{
				if(typeid(*c)==typeid(T))
				{
					index=counter;
					break;
				}
				counter++;
			}
			if(index>=0)
				RemoveComponent(index);
		}

	private:
		bool _isEnabled;
		GameObject* _parent;
		std::vector<GameObject*> _children;
		std::vector<Component*> _components;
	};
}

