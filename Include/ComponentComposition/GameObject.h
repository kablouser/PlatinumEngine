#ifndef PLATINUMENGINE_GAMEOBJECT_H
#define PLATINUMENGINE_GAMEOBJECT_H
#include <string>
#include <vector>
#include "ComponentComposition/Component.h"
namespace PlatinumEngine
{
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

		void AddComponent(Component* component);
		bool HasComponent();
		Component* GetComponent(int index);
		void RemoveComponent(int index);

		bool HasLightComponent();
		bool HasCameraComponent();

	private:
		bool _isEnabled;
		GameObject* _parent;
		std::vector<GameObject*> _children;
		std::vector<Component*> _components;
	};
}
#endif //PLATINUMENGINE_GAMEOBJECT_H
