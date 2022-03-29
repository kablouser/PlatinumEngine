//
// Created by Shihua on 26/03/2022.
//

// Platinum Engine library
#include <SceneManager/HierarchyWindow.h>
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	// ---FUNCTION
	void HierarchyWindow::DisplayTreeNote(GameObject* gameObject, Scene& scene, ModeForDraggingBehavior modeForDraggingBehavior)
	{
		// Store the states (is expanded or not) of the node
		bool is_expanded = ImGui::TreeNodeEx(gameObject,
				ImGuiTreeNodeFlags_FramePadding|(gameObject->GetChildrenCount()==0 ? ImGuiTreeNodeFlags_Leaf : 0),
				"%s", gameObject->name.c_str());

		// Check if this node is clicked
		if(ImGui::IsItemClicked())
			selectedGameObject = gameObject;


		// Add Drag and Drop Events
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{

			// Set payload to carry the game object
			ImGui::SetDragDropPayload("Demo", &gameObject, sizeof(GameObject*));

			// End the DragDropSource
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{

			// Check payload and update the parent of the dropped game object node
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Demo"))
			{

				GameObject* temp = *(GameObject**)payload->Data;

				// check move behavior mode
				// if the mode is to change hierarchy between game objects
				if(modeForDraggingBehavior == _hierarchyMode)
				{
					// change the dragged object's parent
					temp->SetParent(gameObject, scene);
				}
				// if the mode is to change order between game objects
				else
				{
					// check if the two game object have a same parent
					// when the two game objects do not have same a parent
					// change the parent of dragged object to be the same as the target object
					if(temp->GetParent() != gameObject->GetParent())
					{
						// change the dragged object's parent
						temp->SetParent(gameObject->GetParent(), scene);
					}

					// move the position of the dragged objects in the list
					// move the position of objects with no parent
					if(temp->GetParent() == nullptr )
					{
						if(!scene.MoveRootGameObjectPositionInList(gameObject, temp))
							PlatinumEngine::Logger::LogInfo("Cannot move game object to the new position.");

					}
					// move the position of objects with the same parent
					else
					{
						if(!gameObject->GetParent()->MoveChildGameObjectPositionInList(gameObject, temp))
							PlatinumEngine::Logger::LogInfo("Cannot move game object to the new position.");

					}

				}
			}

			// End DragDropTarget
			ImGui::EndDragDropTarget();

		}

		// check if the node is expanded
		if(is_expanded)
		{

			// Loop through the children under this node
			for(int i = 0; i < gameObject->GetChildrenCount(); i++)
			{

				DisplayTreeNote(gameObject->GetChild(i), scene, modeForDraggingBehavior);

			}

			ImGui::TreePop();
		}

	}

	void HierarchyWindow::ShowGUIWindow(bool* isOpen, Scene& scene)
	{

		// Generate the Hierarchy window
		if(ImGui::Begin("Hierarchy Window", isOpen))
		{

			if (ImGui::RadioButton("Change Hierarchy", modeForDraggingBehavior == _hierarchyMode))
			{
				modeForDraggingBehavior = _hierarchyMode;
			}

			ImGui::SameLine();

			if (ImGui::RadioButton("Change Order", modeForDraggingBehavior == _orderMode))
			{
				modeForDraggingBehavior = _orderMode;
			}

			// Loop through every root game objects in a scene
			for(int i =0 ; i< scene.GetRootGameObjectsCount(); i++)
			{

				// Create node for this game object
				DisplayTreeNote(scene.GetRootGameObject(i),scene, modeForDraggingBehavior);

			}

		}
		// End window
		ImGui::End();
	}

	// ---CONSTRUCTOR
	HierarchyWindow::HierarchyWindow():selectedGameObject(nullptr),modeForDraggingBehavior(_orderMode)
	{}


}