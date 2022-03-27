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


namespace PlatinumEngine
{
	typedef struct HierStruct
	{

		std::string ownID;
		std::string parentID;
		std::vector<struct HierStruct> childID;

		explicit HierStruct(const char* newID, const char* newParentID = "")
		{
			ownID = newID;
			parentID = newParentID;

		}
	} HierarchyStruct;

	class HierarchyWindow
	{

		friend class GameObject;

	public:

		// ---FUNCTION
		void ShowGUIWindow(bool* isOpen);

		void GenerateHierarchyListForTest();

		void ShowGUIWindow(bool* isOpen, std::vector<GameObject*>& gameObjectList);

		// ---CONSTRUCTOR
		HierarchyWindow();

	private:

		// ---FUNCTION
		void DisplayTreeNote(std::vector<HierarchyStruct>& hierarchyListParameter);

		void DisplayTreeNote(std::vector<GameObject *>& hierarchyListParameter);

		// ---VARIABLE
		std::vector<HierarchyStruct> hierarchyList;

	};

}

namespace PlatinumEngine
{
	// ---FUNCTION
	void HierarchyWindow::ShowGUIWindow(bool* isOpen)
	{
		if(ImGui::Begin("Hierarchy Window", isOpen))
		{
			DisplayTreeNote(hierarchyList);
		}
		ImGui::End();
	}

	void HierarchyWindow::GenerateHierarchyListForTest()
	{

		for(int i = 0; i<10; i++)
		{

			hierarchyList.emplace_back(HierarchyStruct(std::to_string(i).c_str()));

			for(int j = 0; j < 3 ; j++)
			{

				HierarchyStruct child = HierarchyStruct(std::to_string(i).append(std::to_string(j)).c_str());

				child.parentID = hierarchyList.back().parentID;

				hierarchyList.back().childID.push_back(child);

			}

		}

	}

	void HierarchyWindow::DisplayTreeNote(std::vector<HierarchyStruct>& hierarchyListParameter)
	{

		for(HierarchyStruct& h : hierarchyListParameter)
		{

			bool is_expanded = ImGui::TreeNodeExV(h.ownID.c_str(), ImGuiTreeNodeFlags_FramePadding, "", nullptr);
			ImGui::SameLine();
			ImGui::Selectable(h.ownID.c_str(), false);
			if(is_expanded)
			{

				if (ImGui::BeginDragDropTarget()) {
					// Some processing...
					ImGui::EndDragDropTarget();
				}

				if (ImGui::BeginDragDropSource()) {
					// Some processing...
					ImGui::EndDragDropSource();
				}


				DisplayTreeNote(h.childID);

				ImGui::TreePop();
			}

		}

	}

	void HierarchyWindow::DisplayTreeNote(std::vector<GameObject *>& hierarchyListParameter)
	{

		for(GameObject* h : hierarchyListParameter)
		{

			bool is_expanded = ImGui::TreeNodeExV(h->name.c_str(), ImGuiTreeNodeFlags_FramePadding, " ", nullptr);
			ImGui::SameLine();
			ImGui::Selectable(h->name.c_str(), false);
			if(is_expanded)
			{

				DisplayTreeNote(h->_children);

				ImGui::TreePop();
			}

		}

	}


	void HierarchyWindow::ShowGUIWindow(bool* isOpen, std::vector<GameObject*>& gameObjectList)
	{

		if(ImGui::Begin("Hierarchy Window", isOpen))
		{
			DisplayTreeNote(hierarchyList);
		}
		ImGui::End();

	}

	// ---CONSTRUCTOR
	HierarchyWindow::HierarchyWindow()
	{

		GenerateHierarchyListForTest();


	}


}


