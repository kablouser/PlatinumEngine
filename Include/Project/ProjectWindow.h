//
// Created by Matt on 05/04/2022.
//

#pragma once

#include <imgui.h>
#include <filesystem>
#include <string>
#include <IconsFontAwesome6.h>
#include <Logger/Logger.h>
#include <SceneManager/Scene.h>
#include <Inspector/InspectorWindow.h>

namespace PlatinumEngine
{
	/**
	 * Project window is a GUI that displays the structure of a users assets folder
	 */
	class ProjectWindow
	{
	public:
		ProjectWindow();
		void ShowGUIWindow(bool* isOpen);

	private:
		void ShowTreeNode(std::filesystem::path dir);
		void DragDropMoveRegularFile(std::filesystem::path dir, const ImGuiPayload* payload);
		void ShowProjectWindowPreview(std::filesystem::path filePath);
		std::string FormatFileSize(uintmax_t size, int precision=2);
		void RenderPreview(std::filesystem::path filePath);

		//Preview related
		std::filesystem::path _previewFilePath;
		int _childWindowCount = 1;
		bool _isPreviewEnabled = false;
		Framebuffer _renderTexture;
		int _framebufferWidth = 256;
		int _framebufferHeight = 256;
		EditorCamera _previewCamera;
		Maths::Vec3 _modelRotation;

		//Filesystem related
		enum class MoveType {copy, cut, none};
		MoveType _moveType = MoveType::none;
		std::filesystem::path _moveFilePath;

		//Misc parameters
		std::string _parentFolder = "./Assets";
		std::string _ignoreDatabaseName = "AssetDatabase.csv";
		// Mac has this file called .DS_Store which should also be ignored
		std::string _ignoreMacFolderAttribFile = ".DS_Store";
		std::string _toFind = ""; // The folder or file to find in assets (used for search bar)
	};
}
