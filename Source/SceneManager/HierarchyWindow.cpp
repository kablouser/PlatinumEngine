//
// Created by Shihua on 26/03/2022.
//

// Platinum Engine library
#include <SceneManager/HierarchyWindow.h>
#include <Logger/Logger.h>
#include "ComponentComposition/Transform.h"
#include "ComponentComposition/MeshRender.h"

namespace PlatinumEngine
{
	// ---FUNCTION
	void HierarchyWindow::DisplayTreeNote(GameObject* gameObject, Scene& scene, ModeForDraggingBehavior modeForDraggingBehavior)
	{

		// Store the states (is expanded or not) of the node
		bool is_expanded = ImGui::TreeNodeEx(gameObject,
				ImGuiTreeNodeFlags_FramePadding|(gameObject->GetChildrenCount()==0 ? ImGuiTreeNodeFlags_Leaf : 0),
				"%s","");

		// this is to make sure the selectable block below is on the same line
		ImGui::SameLine();

		// push id of current node
		ImGui::PushID(gameObject);

		// check if the current game object is selected
		if(_sceneEditor->GetSelectedGameobject() == gameObject)
		{
			// set draw layer
			ImGui::GetWindowDrawList()->ChannelsSplit(2);
			ImGui::GetWindowDrawList()->ChannelsSetCurrent(1);
		}

		// create selectable block
		ImGui::Selectable((std::string{ICON_FA_CIRCLE_NODES} + " " + gameObject->name).c_str(), false);

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

		// Handle right click menu for a game object
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("RightClickGameObject");
		}
		if (ImGui::BeginPopup("RightClickGameObject"))
		{
			ImGui::Selectable("Remove Object");
			if (ImGui::IsItemClicked())
			{
				auto selectedObject = _sceneEditor->GetSelectedGameobject();
				if (gameObject == selectedObject)
					_sceneEditor->DeleteSelectedGameObject();
				else
				{
					// For a safe delete, we need to check if the selected game object is a child of the selected
					// object to delete
					if (selectedObject)
					{
						auto parent = _sceneEditor->GetSelectedGameobject()->GetParent();
						while (parent)
						{
							// Manually set nullptr as we know we will remove directly later
							if (parent == gameObject)
								_sceneEditor->SetSelectedGameobject(nullptr);
							parent = parent->GetParent();
						}
					}

					scene.RemoveGameObject(*gameObject);
				}
			}
			ImGui::Selectable("Duplicate Object");
			if (ImGui::IsItemClicked())
			{
				GameObject* duplicateGameObject = scene.AddGameObject(gameObject->name);;
				RenderComponent* gameObjectRC = gameObject->GetComponent<RenderComponent>();
				TransformComponent* gameObjectTC = gameObject->GetComponent<TransformComponent>();

				//Manual copy of parameters
				if(gameObjectRC != nullptr)
				{
					scene.AddComponent<RenderComponent>(duplicateGameObject);
					RenderComponent* dupObjectRC = duplicateGameObject->GetComponent<RenderComponent>();
					dupObjectRC->material.diffuseTexture = gameObjectRC->material.diffuseTexture;
					dupObjectRC->material.normalTexture = gameObjectRC->material.normalTexture;
					dupObjectRC->material.useTexture = gameObjectRC->material.useTexture;
					dupObjectRC->material.useNormalTexture = gameObjectRC->material.useNormalTexture;
					dupObjectRC->material.useBlinnPhong = gameObjectRC->material.useBlinnPhong;
					dupObjectRC->material.shininessFactor = gameObjectRC->material.shininessFactor;
					dupObjectRC->SetMesh(gameObjectRC->GetMesh());
				}
				if(gameObjectTC != nullptr)
				{
					scene.AddComponent<TransformComponent>(duplicateGameObject);
					TransformComponent* dupObjectTC = duplicateGameObject->GetComponent<TransformComponent>();
					dupObjectTC->localPosition = gameObjectTC->localPosition;
					dupObjectTC->localRotation = gameObjectTC->localRotation;
					dupObjectTC->localScale = gameObjectTC->localScale;
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		// Add Drag and Drop Events
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the game object
			ImGui::SetDragDropPayload("DragGameObject", &gameObject, sizeof(GameObject*));

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

				GameObject* payloadPointer = *(GameObject**)payload->Data;

				// check move behavior mode
				// if the mode is to change hierarchy between game objects
				if(modeForDraggingBehavior == _hierarchyMode)
				{

					// check if the dragged objects is the parent or ancestor of the target object
					GameObject* temp = gameObject->GetParent();
					while(temp != nullptr)
					{
						if(temp == payloadPointer)
						{
							PLATINUM_INFO("Setting a parent object as a child of one of it's children or it's children' children is forbidden.");
							break;
						}

						temp = temp->GetParent();
					}

					if(temp == nullptr)
						// change the dragged object's parent
						payloadPointer->SetParent(gameObject, scene);
				}
				// if the mode is to change order between game objects
				else
				{
					// check if the two game object have a same parent
					// when the two game objects do not have same a parent
					// change the parent of dragged object to be the same as the target object
					if(payloadPointer->GetParent() == gameObject->GetParent())
					{
						
						// move the position of the dragged objects in the list
						// move the position of objects with no parent
						if (payloadPointer->GetParent() == nullptr)
						{
							if (!scene.MoveRootGameObjectPositionInList(gameObject, payloadPointer))
								PlatinumEngine::Logger::LogInfo("Cannot move game object to the new position.");

						}
						// move the position of objects with the same parent
						else
						{
							if (!gameObject->GetParent()->MoveChildGameObjectPositionInList(gameObject, payloadPointer))
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
							payloadPointer->SetParent(nullptr, scene);
						}
					}
				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
				{
					char* payloadPointer = (char*)payload->Data;
					int size = payload->DataSize;
					std::string filePath = "";
					for(int i=0;i<size;i++)
						filePath+=*(payloadPointer+i);
					std::filesystem::path payloadPath = std::filesystem::path(filePath);
					if(payloadPath.extension()==".obj")
					{
						GameObject* go = scene.AddGameObject(payloadPath.stem().string());
						scene.AddComponent<Transform>(go);
						scene.AddComponent<MeshRender>(go);
						//Now we set the mesh
						auto asset_Helper = _assetHelper->GetMeshAsset(payloadPath.string());
						if (std::get<0>(asset_Helper))
							go->GetComponent<MeshRender>()->SetMesh(std::get<1>(asset_Helper));
						_sceneEditor->SetSelectedGameobject(go);
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
	HierarchyWindow::HierarchyWindow(SceneEditor* sceneEditor, AssetHelper* assetHelper):_sceneEditor(sceneEditor),_assetHelper(assetHelper),
		_modeForDraggingBehavior(_orderMode)
	{}
}