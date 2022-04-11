//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>

using namespace PlatinumEngine;

InspectorWindow::InspectorWindow(AssetHelper* assetHelper):_assetHelper(assetHelper) {}
void InspectorWindow::ShowGUIWindow(bool* isOpen, Scene& scene)
{
	if(ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector Window", isOpen))
	{
		if (_activeGameObject)
		{
			ImGui::Text("Name: ");
			ImGui::SameLine();
			static char objectNameBuffer[128];
			strcpy(objectNameBuffer, _activeGameObject->name.c_str());
			ImGui::InputText("##input name", objectNameBuffer, IM_ARRAYSIZE(objectNameBuffer));
			_activeGameObject->name = std::string{ objectNameBuffer };

			ImGui::SameLine();

			if (ImGui::Checkbox("##IsEnabled", &_isObjectEnabled))
			{
				_activeGameObject->SetEnabled(_isObjectEnabled, scene);
			}

			// Now render each component gui
			if (_activeGameObject->GetComponent<RenderComponent>() != nullptr)
				ShowMeshRenderComponent(scene);

			if (_activeGameObject->GetComponent<TransformComponent>() != nullptr)
				ShowTransformComponent(scene);

			ImGui::Separator();
			if (_isAddComponentWindowOpen)
				ShowAddComponent(scene);

			if (ImGui::Button("Add Component"))
			{
				_isAddComponentWindowOpen = !_isAddComponentWindowOpen;
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Add Component")) _isAddComponentWindowOpen = !_isAddComponentWindowOpen;
				ImGui::EndPopup();
			}
		}
	}
	ImGui::End();
}

void InspectorWindow::SetActiveGameObject(GameObject* gameObject)
{
	_activeGameObject = gameObject;

	// Get states of gameObject if not null
	if (gameObject)
	{
		_isObjectEnabled = gameObject->IsEnabled();
	}
}

void InspectorWindow::ShowMeshRenderComponent(Scene& scene)
{

	ImGui::Separator();
	char meshBuffer[64];
	bool isHeaderOpen = ImGui::CollapsingHeader("Mesh Render Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRenderComponent")) {
		// remove component
		scene.RemoveComponent(*_activeGameObject->GetComponent<RenderComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Mesh");
		ImGui::SameLine();
		ImGui::PushItemWidth(160.0f);

		// store the current mesh name into mesh buffer, so that we can display it in the input text box
		if(_activeGameObject->GetComponent<RenderComponent>()->GetMesh() != nullptr)
			strcpy(meshBuffer,  _activeGameObject->GetComponent<RenderComponent>()->GetMesh()->fileName.c_str());
		else
			memset(meshBuffer, 0, 64 * sizeof(char));

		// show text box (read only)
		ImGui::InputText("##Mesh Name",meshBuffer,sizeof(meshBuffer), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if(ImGui::Button("Choose mesh"))
		{
			ImGui::OpenPopup("Select Mesh");
		}
		if (ImGui::BeginDragDropTarget())
		{
			// Check payload and update the parent of the dropped game object node
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshPayload"))
			{
				Mesh* payloadPointer = *(Mesh**)payload->Data;


			}

			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
	}

		auto asset_Helper = _assetHelper->ShowGuiWindow();
		if(std::get<0>(asset_Helper) == true)
		{
			_activeGameObject->GetComponent<RenderComponent>()->SetMesh(std::get<1>(asset_Helper));
		}
	}
}

void InspectorWindow::ShowTransformComponent(Scene& scene)
{
	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveTransformComponent")) {
		// remove component
		scene.RemoveComponent(*_activeGameObject->GetComponent<TransformComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xpos", &_activeGameObject->GetComponent<TransformComponent>()->position[0], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ypos", &_activeGameObject->GetComponent<TransformComponent>()->position[1], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zpos", &_activeGameObject->GetComponent<TransformComponent>()->position[2], 0.001f);

		static float eulerRotation[3] = {0.0f, 0.0f, 0.0f};
		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xrot", &eulerRotation[0], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Yrot", &eulerRotation[1], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zrpt", &eulerRotation[2], 0.001f);
		_activeGameObject->GetComponent<TransformComponent>()->rotation = Maths::Quaternion::EulerToQuat(
				Maths::Vec3(eulerRotation[0], eulerRotation[1], eulerRotation[2]));

		ImGui::Text("Scale:    ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xscale", &_activeGameObject->GetComponent<TransformComponent>()->scale[0], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Yscale", &_activeGameObject->GetComponent<TransformComponent>()->scale[1], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zscale", &_activeGameObject->GetComponent<TransformComponent>()->scale[2], 0.001f);
		ImGui::PopItemWidth();
	}
}

void InspectorWindow::ShowAddComponent(Scene& scene)
{
	if (ImGui::BeginChild("ComponentSelector"))
	{
		const char* components[] = { "Mesh Render Component", "Transform Component"};
		static const char* selectedComponent = nullptr;
		static char componentSelectorBuffer[128];
		ImGui::Text("%s", "Select a Component");
		ImGui::InputText("##FilterComponents", componentSelectorBuffer, IM_ARRAYSIZE(componentSelectorBuffer));

		if (ImGui::BeginListBox("##ComponentSelector"))
		{
			// Add components to list
			for (unsigned int i = 0; i < IM_ARRAYSIZE(components); ++i)
			{
				// Only add components that match search string
				if (std::string(components[i]).find(std::string(componentSelectorBuffer)) != std::string::npos)
				{
					// If found, display component
					if (ImGui::Selectable(components[i]))
					{
						selectedComponent = components[i];
					}
				}
			}

			ImGui::EndListBox();
		}

		// Cancel button
		if (ImGui::Button("Cancel"))
		{
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}

		if (selectedComponent)
		{
			if (strcmp(selectedComponent, "Mesh Render Component") == 0)
			{
				// Add Render Component
				scene.AddComponent<RenderComponent>(_activeGameObject);
			}
			else if (strcmp(selectedComponent, "Transform Component") == 0)
			{
				// Add Transform Component
				scene.AddComponent<TransformComponent>(_activeGameObject);
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
		ImGui::EndChild();
	}
}
