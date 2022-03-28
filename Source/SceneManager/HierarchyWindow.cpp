//
// Created by Shihua on 26/03/2022.
//

// Platinum Engine library
#include <SceneManager/HierarchyWindow.h>

namespace PlatinumEngine
{
	// ---FUNCTION
	void HierarchyWindow::DisplayTreeNote(GameObject* gameObject, Scene& scene)
	{
		// Store the states (is expanded or not) of the node
		bool is_expanded = ImGui::TreeNodeExV(gameObject,
				ImGuiTreeNodeFlags_FramePadding|(gameObject->GetChildrenCount()==0 ? ImGuiTreeNodeFlags_Leaf : 0),
				gameObject->name.c_str(), nullptr);

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
				GameObject* temp = *(GameObject**) payload->Data;
				temp->SetParent(gameObject,scene);
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

				DisplayTreeNote(gameObject->GetChild(i), scene);

			}

			ImGui::TreePop();
		}

	}

	void HierarchyWindow::ShowGUIWindow(bool* isOpen, Scene& scene)
	{

		// Generate the Hierarchy window
		if(ImGui::Begin("Hierarchy Window", isOpen))
		{

			// Loop through every root game objects in a scene
			for(int i =0 ; i< scene.GetRootGameObjectsCount(); i++)
			{

				// Create node for this game object
				DisplayTreeNote(scene.GetRootGameObject(i),scene);

			}

		}
		// End window
		ImGui::End();
	}

	// ---CONSTRUCTOR
	HierarchyWindow::HierarchyWindow():selectedGameObject(nullptr)
	{}


}