//
// Created by Matt on 27/03/2022.
//

#pragma once

#include "WindowManager/Filedialog.h"
#include <ComponentComposition/Component.h>
#include <ComponentComposition/RenderComponent.h>
#include <ComponentComposition/TransformComponent.h>
#include <ComponentComposition/GameObject.h>
#include <SceneManager/SceneManager.h>
#include <OpenGL/Mesh.h>

#include <IconsFontAwesome6.h>
#include <IconsMaterialDesign.h>
namespace PlatinumEngine
{
	class InspectorWindow
	{
	public:
		InspectorWindow(AssetHelper* assetHelper);
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
		AssetHelper* _assetHelper;
		GameObject* _activeGameObject = nullptr;
		std::string _meshFileName;
		bool _isAddComponentWindowOpen = false;

		// Have to keep track of if object enabled ourselves as isEnabled is a private member of game object
		bool _isObjectEnabled;

		//ImGui helper parameters
		float _sameLineMesh = 70.f;
		float _itemWidthMesh = 160.f;
		float _sameLineTransform = 70.f;
	};
}