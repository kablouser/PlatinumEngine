//
// Created by Shihua on 26/03/2022.
//
#pragma once

// C++ standard library
#include <string>
#include <vector>
#include <algorithm>

// ImGui library

#include <imgui.h>
// Icon library

// Platinum Engine library
#include <ComponentComposition/GameObject.h>
#include <SceneManager/Scene.h>
#include <SceneEditor/SceneEditor.h>
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	class HierarchyWindow
	{
	public:

		// ---> FUNCTION
		/**
		 * Function that show Hierarchy window
		 * @param isOpen : status of this window (open or not)
		 * @param scene : scene object reference that carries the game object hierarchy structure
		 */
		void ShowGUIWindow(bool* isOpen, Scene& scene);

		// ---> CONSTRUCTOR
		HierarchyWindow(SceneEditor* sceneEditor);

		// ---> PARAMETER

	private:
		// ---> ENUM
		enum ModeForDraggingBehavior{_noneMode, _hierarchyMode, _orderMode};


		// ---> FUNCTION
		/**
		 * Function to display nested tree node for all the game objects in the scene
		 * @param gameObject : the game object that is processing
		 * @param scene : scene object reference that carries the game object hierarchy structure
		 */
		void DisplayTreeNote(
				SavedReference<GameObject>& gameObject,
				Scene& scene,
				ModeForDraggingBehavior modeForDraggingBehavior);

		// ---> PARAMETER
		ModeForDraggingBehavior _modeForDraggingBehavior;
		SceneEditor* _sceneEditor;
	};
}