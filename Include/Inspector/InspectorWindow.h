//
// Created by Matt on 27/03/2022.
//

#pragma once

#include <imgui.h>
#include "WindowManager/Filedialog.h"
#include <ComponentComposition/Component.h>
#include <ComponentComposition/RenderComponent.h>
#include <ComponentComposition/TransformComponent.h>
#include <ComponentComposition/GameObject.h>
#include <SceneManager/SceneManager.h>

namespace PlatinumEngine
{
	class InspectorWindow
	{
	public:
		InspectorWindow() = default;
		void ShowGUIWindow(bool* isOpen, Scene& scene);
		void SetActiveGameObject(GameObject* gameObject);
	private:
		// TODO: Add specific component guis as components are created
		void ShowMeshRenderComponent(Scene& scene);
		void ShowTransformComponent(Scene& scene);
		void ShowCameraComponent(Scene& scene);

		// Shown when add component button pressed
		void ShowAddComponent(Scene& scene);
	private:
		GameObject* _activeGameObject = nullptr;
		std::string _meshFileName;
		bool _isAddComponentWindowOpen = false;

		// Have to keep track of if object enabled ourselves as isEnabled is a private member of game object
		bool _isObjectEnabled;
	};
}