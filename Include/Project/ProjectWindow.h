//
// Created by Matt on 05/04/2022.
//

#pragma once

#include <imgui.h>
#include <filesystem>
#include <string>
#include <IconsFontAwesome6.h>
#include "Logger/Logger.h"
#include "AssetDatabase/AssetHelper.h"
#include "SceneManager/Scene.h"
#include "Inspector/InspectorWindow.h"

namespace PlatinumEngine
{
	/**
	 * Project window is a GUI that displays the structure of a users assets folder
	 */
	class ProjectWindow
	{
	public:
		ProjectWindow(Scene* scene, AssetHelper* assetHelper, SceneEditor* sceneEditor);
		void ShowGUIWindow(bool* isOpen);



	private:
		void ShowTreeNode(std::filesystem::path dir);
		void DragDropMoveRegularFile(std::filesystem::path dir, const ImGuiPayload* payload);

	private:
		Scene* _scene;
		SceneEditor* _sceneEditor;
		AssetHelper* _assetHelper;
		std::string _parentFolder = "./Assets";
		std::string _ignoreDatabaseName = "AssetDatabase.csv";
		// Mac has this file called .DS_Store which should also be ignored
		std::string _ignoreMacFolderAttribFile = ".DS_Store";
		std::string _toFind = ""; // The folder or file to find in assets (used for search bar)
	};
}
