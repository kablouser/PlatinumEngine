//
// Created by Shihua on 26/03/2022.
//

// Platinum Engine library
#include <SceneManager/HierarchyWindow.h>
#include <Logger/Logger.h>
#include "ComponentComposition/TransformComponent.h"
#include "ComponentComposition/RenderComponent.h"

namespace PlatinumEngine
{
	// ---FUNCTION
	void HierarchyWindow::DisplayTreeNote(
			SavedReference<GameObject>& gameObject,
			Scene& scene,
			ModeForDraggingBehavior modeForDraggingBehavior)
	{
		if (!gameObject)
			return;

		// Store the states (is expanded or not) of the node
		bool is_expanded = ImGui::TreeNodeEx(gameObject.pointer.get(),
				ImGuiTreeNodeFlags_FramePadding|
				(gameObject.pointer->GetChildrenCount()==0 ? ImGuiTreeNodeFlags_Leaf : 0),
				"%s","");

		// this is to make sure the selectable block below is on the same line
		ImGui::SameLine();

		// push id of current node
		ImGui::PushID(gameObject.pointer.get());

		// check if the current game object is selected
		if(_sceneEditor->GetSelectedGameobject() == gameObject)
		{
			// set draw layer
			ImGui::GetWindowDrawList()->ChannelsSplit(2);
			ImGui::GetWindowDrawList()->ChannelsSetCurrent(1);
		}

		// create selectable block
		ImGui::Selectable((std::string(ICON_FA_CIRCLE_NODES) + " " + gameObject.pointer->name).c_str(), false);

		// check if the current game object is selected
		if(_sceneEditor->GetSelectedGameobject() == gameObject)
		{
			// highlight the selected selectable block
			ImGui::GetWindowDrawList()->ChannelsSetCurrent(0);
			ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]));
			ImGui::GetWindowDrawList()->ChannelsMerge();
		}

		// Check if this node is clicked
		if(ImGui::IsItemClicked())
		{
			_sceneEditor->SetSelectedGameobject(gameObject);
		}


		// Add Drag and Drop Events
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the game object
			ImGui::SetDragDropPayload(
					"DragGameObject",
					// pointer to SavedReference (id and pointer)
					&gameObject,
					// same as sizeof(SavedReference), not sizeof(SavedReference&)
					sizeof(gameObject));

			// End the DragDropSource
			ImGui::EndDragDropSource();
		}

		// pop out ID
		ImGui::PopID();

		if (ImGui::BeginDragDropTarget())
		{

			// Check payload and update the parent of the dropped game object node
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragGameObject"))
			{

				SavedReference<GameObject> payloadPointer = *(SavedReference<GameObject>*)payload->Data;

				// check move behavior mode
				// if the mode is to change hierarchy between game objects
				if(modeForDraggingBehavior == _hierarchyMode)
				{

					// check if the dragged objects is the parent or ancestor of the target object
					SavedReference<GameObject>& temp = gameObject.pointer->GetParent();
					while(temp)
					{
						if(temp == payloadPointer)
						{
							PLATINUM_INFO("Setting a parent object as a child of one of it's children or it's children' children is forbidden.");
							break;
						}

						temp = temp.pointer->GetParent();
					}

					if(!temp) // if null
						// change the dragged object's parent
						payloadPointer.pointer->SetParent(gameObject, scene);
				}
				// if the mode is to change order between game objects
				else
				{
					// check if the two game object have a same parent
					// when the two game objects do not have same a parent
					// change the parent of dragged object to be the same as the target object
					if(payloadPointer.pointer->GetParent() == gameObject.pointer->GetParent())
					{

						// move the position of the dragged objects in the list
						// move the position of objects with no parent
						if (!payloadPointer.pointer->GetParent())
						{
							if (!scene.MoveRootGameObjectPositionInList(gameObject, payloadPointer))
								PlatinumEngine::Logger::LogInfo("Cannot move game object to the new position.");

						}
						// move the position of objects with the same parent
						else
						{
							if (!gameObject.pointer->GetParent().pointer->
								MoveChildGameObjectPositionInList(gameObject, payloadPointer))
								PlatinumEngine::Logger::LogInfo("Cannot move game object to the new position.");
						}
					}
				}
			}
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshPathPayload"))
			{
				char* payloadPointer = (char*)payload->Data;
				int size = payload->DataSize;
				std::cout<<"SIZE: "<<size<<std::endl;
			}

			// End DragDropTarget
			ImGui::EndDragDropTarget();

		}

		// check if the node is expanded
		if(is_expanded)
		{

			// Loop through the children under this node
			for(int i = 0; i < gameObject.pointer->GetChildrenCount(); i++)
			{

				DisplayTreeNote(gameObject.pointer->GetChild(i), scene, modeForDraggingBehavior);

			}

			ImGui::TreePop();
		}
	}

	void HierarchyWindow::ShowGUIWindow(bool* isOpen, Scene& scene)
	{

		// Generate the Hierarchy window
		if(ImGui::Begin(ICON_FA_BARS_STAGGERED " Hierarchy Window", isOpen))
		{

			// Gui for choosing hierarchy behaviour mode
			if (ImGui::RadioButton("Change Hierarchy", _modeForDraggingBehavior == _hierarchyMode))
			{
				_modeForDraggingBehavior = _hierarchyMode;
			}

			ImGui::SameLine();

			if (ImGui::RadioButton("Change Order", _modeForDraggingBehavior == _orderMode))
			{
				_modeForDraggingBehavior = _orderMode;
			}

			// Loop through every root game objects in a scene
			for(int i =0 ; i< scene.GetRootGameObjectsCount(); i++)
			{

				// Create node for this game object
				DisplayTreeNote(scene.GetRootGameObject(i),scene, _modeForDraggingBehavior);

			}


			// Create Invisible button for achieving dragging tree node to the root game object list behavior
			ImVec2 windowSize = ImGui::GetContentRegionAvail();

			ImGui::InvisibleButton(" ", ImVec2(windowSize.x,150));

			// Add Drop Events
			if (ImGui::BeginDragDropTarget())
			{

				// Check payload and update the parent of the dropped game object node
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragGameObject"))
				{
					if(payload->Data != nullptr)
					{
						GameObject* payloadPointer = *(GameObject**)payload->Data;

						// check move behavior mode
						// if the mode is to change hierarchy between game objects
						if (_modeForDraggingBehavior == _hierarchyMode)
						{
							// change the dragged object's parent
							payloadPointer->SetParent({}, scene);
						}
					}
				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
				{
					char* payloadPointer = (char*)payload->Data;
					int size = payload->DataSize;
					std::string filePath;
					for(int i=0;i<size;i++)
						filePath+=*(payloadPointer+i);
					std::filesystem::path payloadPath = std::filesystem::path(filePath);
					if(payloadPath.extension()==".obj")
					{
						std::string name = payloadPath.stem().string();
						SavedReference<GameObject> go = scene.AddGameObject(name);
						scene.AddComponent<TransformComponent>(go);
						scene.AddComponent<RenderComponent>(go);
					}
				}
				// End DragDropTarget
				ImGui::EndDragDropTarget();
			}
		}
		// End window
		ImGui::End();
	}

	// ---CONSTRUCTOR
	HierarchyWindow::HierarchyWindow(SceneEditor* sceneEditor):_sceneEditor(sceneEditor),
		_modeForDraggingBehavior(_orderMode)
	{}
}