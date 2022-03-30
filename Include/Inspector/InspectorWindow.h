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
		explicit InspectorWindow(Scene &scene);
		void ShowGUIWindow(bool* isOpen);
		void SetActiveGameObject(GameObject* gameObject);
	private:
		// TODO: Add specific component guis as components are created
		void ShowMeshRenderComponent();
		void ShowTransformComponent();

		// Shown when add component button pressed
		void ShowAddComponent();
	private:
		Scene &_scene;
		GameObject* _activeGameObject = nullptr;
		std::string _meshFileName;
		bool _isAddComponentWindowOpen = false;
	};
}