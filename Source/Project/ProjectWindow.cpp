//
// Created by Matt on 05/04/2022.
//

#include <Project/ProjectWindow.h>
#include <iostream>

using namespace PlatinumEngine;

void ProjectWindow::ShowGUIWindow(bool* isOpen)
{
	if (ImGui::Begin(ICON_FA_FOLDER " Project Window"))
	{
		static char assetSelectorBuffer[128];
		ImGui::InputText("##FilterAssets", assetSelectorBuffer, IM_ARRAYSIZE(assetSelectorBuffer));
		ImGui::SameLine();
		ImGui::Text("%s", ICON_KI_SEARCH);

		// If we don't have something to search for, use default assets folder
		_toFind = assetSelectorBuffer;
		if (!_toFind.empty())
		{
			// Recursively search for the target string in each directory
			for (std::filesystem::recursive_directory_iterator i(_parentFolder), end; i != end; ++i)
			{
				// Find search string in filename, also check if parent path contains search string to avoid adding twice
				if (i->path().filename().string().find(_toFind) != std::string::npos && (i->path().parent_path().string().find(_toFind) == std::string::npos))
				{
					ShowTreeNode(i->path());
				}
			}
		}
		else
		{
			ShowTreeNode(std::filesystem::path(_parentFolder));
		}
	}

	ImGui::End();
}

/**
 * This is a recursive function that will show a tree structure from a given parent directory
 * @param dir the current directory level
 */
void ProjectWindow::ShowTreeNode(std::filesystem::path dir)
{
	// Common flags for all items in tree
	auto flags = ImGuiTreeNodeFlags_FramePadding;

	// If a directory, need to loop its sub-contents
	if (is_directory(dir))
	{
		// Hide label of node using "##" (this is so we can have fancy icons)
		bool isExpanded = ImGui::TreeNodeEx((std::string{"##"} + dir.filename().string()).c_str(), flags);

		// Only show sub-contents if tree node is expanded
		if (isExpanded)
		{
			// Add open folder icon and name
			ImGui::SameLine();
			ImGui::Text("%s", (std::string{ICON_FA_FOLDER_OPEN} + " " + dir.filename().string()).c_str());
			for (std::filesystem::directory_iterator i(dir), end; i != end; ++i)
			{
				ShowTreeNode(i->path());
			}

			ImGui::TreePop();
		}
		else
		{
			// Add closed folder icon and name
			ImGui::SameLine();
			ImGui::Text("%s", (std::string{ICON_FA_FOLDER} + " " + dir.filename().string()).c_str());
		}
	}
	else
	{
		// Do not want to add the asset database to the tree
		if (dir != _ignoreDatabaseName)
		{
			// A path is a leaf of a tree (i.e. it cannot be expanded)
			ImGui::TreeNodeEx(dir.filename().string().c_str(), flags | ImGuiTreeNodeFlags_Leaf);
			ImGui::TreePop();

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				// Set payload to carry the filepath
				//TODO: if(type is Mesh) then we set payload data to the full path [Need to see how to check type also]
					std::string fp = dir.string();
					ImGui::SetDragDropPayload("MeshPathPayload", fp.c_str(), fp.length());
				ImGui::EndDragDropSource();
			}
		}
	}
}
