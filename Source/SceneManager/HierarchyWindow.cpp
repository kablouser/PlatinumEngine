//
// Created by Shihua on 26/03/2022.
//

// Platinum Engine library
#include <SceneManager/HierarchyWindow.h>
#include <Logger/Logger.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/MeshRender.h>
#include <Application.h>

#include <AssetDatabase/AssetHelper.h>
#include <SceneManager/SceneWithTemplates.h>

namespace PlatinumEngine
{
	// ---FUNCTION
	void HierarchyWindow::DisplayTreeNote(
			SavedReference<GameObject>& gameObject,
			ModeForDraggingBehavior modeForDraggingBehavior)
	{
		if (!gameObject)
			return;

		// Store the states (is expanded or not) of the node
		bool is_expanded = ImGui::TreeNodeEx(gameObject.DeRef().get(),
				ImGuiTreeNodeFlags_FramePadding|
				(gameObject.DeRef()->GetChildrenCount()==0 ? ImGuiTreeNodeFlags_Leaf : 0),
				"%s","");

		// this is to make sure the selectable block below is on the same line
		ImGui::SameLine();

		// push id of current node
		ImGui::PushID(gameObject.DeRef().get());

		// check if the current game object is selected
		if(Application::Instance->sceneEditor.GetSelectedGameobject() == gameObject)
		{
			// set draw layer
			ImGui::GetWindowDrawList()->ChannelsSplit(2);
			ImGui::GetWindowDrawList()->ChannelsSetCurrent(1);
		}

		// create selectable block
		ImGui::Selectable((std::string(ICON_FA_CIRCLE_NODES) + " " + gameObject.DeRef()->name).c_str(), false);

		// check if the current game object is selected
		if(Application::Instance->sceneEditor.GetSelectedGameobject() == gameObject)
		{
			// highlight the selected selectable block
			ImGui::GetWindowDrawList()->ChannelsSetCurrent(0);
			ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]));
			ImGui::GetWindowDrawList()->ChannelsMerge();
		}

		// Check if this node is clicked
		if(ImGui::IsItemClicked()||ImGui::IsItemClicked(1))
		{
			Application::Instance->sceneEditor.SetSelectedGameobject(gameObject);
		}

		// Handle right click menu for a game object
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("RightClickGameObject");
		}
		if (ImGui::BeginPopup("RightClickGameObject"))
		{
			ImGui::Text("Remove Object");
			if (ImGui::IsItemClicked())
			{
				auto selectedObject = Application::Instance->sceneEditor.GetSelectedGameobject();
				if (gameObject == selectedObject)
					Application::Instance->sceneEditor.DeleteSelectedGameObject();
			}

			/*ImGui::Separator();
			ImGui::Text("Copy Object");
			if (ImGui::IsItemClicked())
			{

			}*/

			ImGui::EndPopup();
		}


		// Add Drag and Drop Events
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the game object
			ImGui::SetDragDropPayload(
					"DragGameObject",
					// pointer to SavedReference (id and pointer)
					&gameObject.id,
					// same as sizeof(SavedReference), not sizeof(SavedReference&)
					sizeof(gameObject.id));

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

				SavedReference<GameObject> payloadPointer;
				payloadPointer.id = *(IDSystem::ID*)payload->Data;
				payloadPointer.OnIDSystemUpdate(Application::Instance->idSystem);

				// check move behavior mode
				// if the mode is to change hierarchy between game objects
				if(modeForDraggingBehavior == _hierarchyMode)
				{

					// check if the dragged objects is the parent or ancestor of the target object
					SavedReference<GameObject>& temp = gameObject.DeRef()->GetParent();
					while(temp)
					{
						if(temp == payloadPointer)
						{
							PLATINUM_INFO("Setting a parent object as a child of one of it's children or it's children' children is forbidden.");
							break;
						}

						temp = temp.DeRef()->GetParent();
					}

					if(!temp) // if null
						// change the dragged object's parent
						payloadPointer.DeRef()->SetParent(gameObject);
				}
				// if the mode is to change order between game objects
				else
				{
					// check if the two game object have a same parent
					// when the two game objects do not have same a parent
					// change the parent of dragged object to be the same as the target object
					if(payloadPointer.DeRef()->GetParent() == gameObject.DeRef()->GetParent())
					{

						// move the position of the dragged objects in the list
						// move the position of objects with no parent
						if (!payloadPointer.DeRef()->GetParent())
						{
							if (!Application::Instance->scene.MoveRootGameObjectPositionInList(gameObject, payloadPointer))
								PlatinumEngine::Logger::LogInfo("Cannot move game object to the new position.");

						}
						// move the position of objects with the same parent
						else
						{
							if (!gameObject.DeRef()->GetParent().DeRef()->
								MoveChildGameObjectPositionInList(gameObject, payloadPointer))
								PlatinumEngine::Logger::LogInfo("Cannot move game object to the new position.");
						}
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
			for(int i = 0; i < gameObject.DeRef()->GetChildrenCount(); i++)
			{

				DisplayTreeNote(gameObject.DeRef()->GetChild(i), modeForDraggingBehavior);

			}

			ImGui::TreePop();
		}
	}

	void HierarchyWindow::ShowGUIWindow(bool* isOpen)
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
			for(int i =0 ; i< Application::Instance->scene.GetRootGameObjectsCount(); i++)
			{

				// Create node for this game object
				DisplayTreeNote(Application::Instance->scene.GetRootGameObject(i), _modeForDraggingBehavior);

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
						SavedReference<GameObject> payloadPointer;
						payloadPointer.id = *(IDSystem::ID*)payload->Data;
						payloadPointer.OnIDSystemUpdate(Application::Instance->idSystem);

						// check move behavior mode
						// if the mode is to change hierarchy between game objects
						if (_modeForDraggingBehavior == _hierarchyMode)
						{
							// change the dragged object's parent
							payloadPointer.DeRef()->SetParent({});
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
						SavedReference<GameObject> go = Application::Instance->scene.AddGameObject(name);
						Application::Instance->scene.AddComponent<Transform>(go);
						Application::Instance->scene.AddComponent<MeshRender>(go);

						auto asset_Helper = AssetHelper::GetAsset<Mesh>(payloadPath.string());
						if (std::get<0>(asset_Helper))
							go.DeRef()->GetComponent<MeshRender>().DeRef()->SetMesh(std::get<1>(asset_Helper));
						Application::Instance->sceneEditor.SetSelectedGameobject(go);
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
	HierarchyWindow::HierarchyWindow():	_modeForDraggingBehavior(_orderMode)
	{}
}