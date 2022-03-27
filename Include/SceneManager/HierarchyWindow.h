//
// Created by Shihua on 26/03/2022.
//
#pragma once

// C++ standard library
#include <string>
#include <vector>

// ImGui library
#include <imgui.h>

// Platinum Engine library
#include <ComponentComposition/GameObject.h>
#include <SceneManager/Scene.h>
#include <Logger/Logger.h>
#include <algorithm>

namespace PlatinumEngine
{
	class HierarchyWindow
	{
	public:

		// ---> FUNCTION
		void ShowGUIWindow(bool* isOpen, Scene& scene);

		// ---> CONSTRUCTOR
		HierarchyWindow();

		// ---> PARAMETER
		GameObject* selectedGameObject;

	private:

		// ---> FUNCTION
		void DisplayTreeNote(GameObject* gameObject, Scene& scene);

	};

}




