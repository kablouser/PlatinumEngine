#ifndef PLATINUMENGINE_GAMEOBJECT_H
#define PLATINUMENGINE_GAMEOBJECT_H
#include <string>
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
		void AddComponent(Component component);
		bool HasComponent();
		Component GetComponent();
		void RemoveComponent();

		bool HasLightComponent();
		bool HasCameraComponent();

	private:
		Component _component;
		bool _hasComponent;
	};
}
#endif //PLATINUMENGINE_GAMEOBJECT_H
