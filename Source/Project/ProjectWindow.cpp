//
// Created by Matt on 05/04/2022.
//

#include <Project/ProjectWindow.h>


using namespace PlatinumEngine;

void ProjectWindow::ShowGUIWindow(bool* isOpen)
{
	if (ImGui::Begin("Project Window"))
	{
		ShowTreeNode(std::filesystem::path(_parentFolder));
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
		// Only show sub-contents if tree node is expanded
		bool isExpanded = ImGui::TreeNodeEx(dir.filename().c_str(), flags);
		if (isExpanded)
		{
			for (std::filesystem::directory_iterator i(dir), end; i != end; ++i)
			{
				ShowTreeNode(i->path());
			}

			ImGui::TreePop();
		}
	}
	else
	{
		// Do not want to add the asset database to the tree
		if (dir != _ignoreDatabaseName)
		{
			// A path is a leaf of a tree (i.e. it cannot be expanded)
			ImGui::TreeNodeEx(dir.filename().c_str(), flags | ImGuiTreeNodeFlags_Leaf);
			ImGui::TreePop();
		}
	}
}
