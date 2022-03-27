//
// Created by Matt on 27/03/2022.
//

#pragma once

#include <imgui.h>
#include "WindowManager/Filedialog.h"
#include <ComponentComposition/GameObject.h>

namespace PlatinumEngine
{
	class InspectorWindow
	{
	public:
		InspectorWindow() = default;
		void ShowGUIWindow(bool* isOpen);
		void SetActiveGameObject(GameObject* gameObject);
	private:
		// TODO: Add specific component guis as components are created
		void ShowMeshComponent();
		void ShowTransformComponent();
	private:
		GameObject* _activeGameObject = nullptr;

		ImVec2 maxSize = ImVec2((float)1080, (float)960);
		ImVec2 minSize = ImVec2((float)540, (float)480);

		std::string fileName;
	};


	// FOR TESTING
	class MeshComponent : public Component
	{
	public:
		MeshComponent() = default;
	};

	class TransformComponent : public Component
	{
	public:
		TransformComponent() = default;
		float position[3] = {0.0f, 0.0f, 0.0f};
		float rotation[3] = {0.0f, 0.0f, 0.0f};
		float scale[3] = {1.0f, 1.0f, 1.0f};
	};

	class AnotherComponent : public Component
	{
	public:
		AnotherComponent() = default;
	};
}