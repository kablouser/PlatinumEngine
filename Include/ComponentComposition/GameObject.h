#ifndef PLATINUMENGINE_GAMEOBJECT_H
#define PLATINUMENGINE_GAMEOBJECT_H
#include <string>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include "ComponentComposition/Component.h"
#include "iostream"
#include <map>

#include "ComponentComposition/ComponentType.h"

namespace PlatinumEngine
{
	static std::map<std::type_index, std::string> typeToStringMap = {
			{std::type_index(typeid(Component)), "Component" },
			{std::type_index(typeid(MeshComponent)), "MeshComponent"},
			{std::type_index(typeid(LightComponent)), "LightComponent"}};

	class GameObject
	{
	public:
		std::string name;
	public:
		GameObject();
		GameObject(std::string name);

		bool IsEnabled();
		void SetEnabled(bool enableFlag);

		GameObject* GetParent();
		void SetParent(GameObject* parent);

		int GetChildrenCount();
		GameObject* GetChild(int index);

		Component* GetComponent(int index);
		void RemoveComponent(int index);
		int GetComponentCount();

		template <class T> void AddComponent (T *component)
		{
			if(!HasComponent<T>())
				_components.emplace_back(component);
		}

		template<class T> bool HasComponent()
		{
			std::string otherComponentType = typeToStringMap[std::type_index(typeid(T))];
			for(auto& c:_components)
			{
				std::string mappedType = typeToStringMap[std::type_index(typeid(*c))];
				if(mappedType==otherComponentType)
					return true;
			}
			return false;
		}

		template<class T> T* GetComponent()
		{
			std::string otherType = typeToStringMap[std::type_index(typeid(T))];
			for(auto& c:_components)
			{
				std::string mappedType = typeToStringMap[std::type_index(typeid(*c))];
				if(mappedType==otherType)
					return dynamic_cast<T*>(c);
			}
			return NULL;
		}

		template<class T> void RemoveComponent()
		{
			std::string otherComponentType = typeToStringMap[std::type_index(typeid(T))];
			int index=-1, counter=0;
			for(auto& c:_components)
			{
				std::string mappedType = typeToStringMap[std::type_index(typeid(*c))];
				if(mappedType==otherComponentType)
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
#endif //PLATINUMENGINE_GAMEOBJECT_H
