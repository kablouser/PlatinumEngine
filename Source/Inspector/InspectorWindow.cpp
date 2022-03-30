//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>

using namespace PlatinumEngine;

InspectorWindow::InspectorWindow(Scene& scene) : _scene(scene)
{
}

void InspectorWindow::ShowGUIWindow(bool* isOpen)
{
	ImGui::Begin("Inspector Window", isOpen);

	if (_activeGameObject)
	{
		ImGui::Text("Object Name: ");
		ImGui::SameLine();
		static char objectNameBuffer[128];
		strcpy(objectNameBuffer, _activeGameObject->name.c_str());
		ImGui::InputText("##input name", objectNameBuffer, IM_ARRAYSIZE(objectNameBuffer));
		_activeGameObject->name = std::string{objectNameBuffer};

		// Now render each component gui
		if (_activeGameObject->GetComponent<RenderComponent>()!= nullptr)
			ShowMeshRenderComponent();

		if (_activeGameObject->GetComponent<TransformComponent>()!= nullptr)
			ShowTransformComponent();

		ImGui::Separator();
		if (_isAddComponentWindowOpen)
			ShowAddComponent();
		if (ImGui::Button("Add Component")) {
			_isAddComponentWindowOpen = !_isAddComponentWindowOpen;
		}
	}

	ImGui::End();
}

void InspectorWindow::SetActiveGameObject(GameObject* gameObject)
{
	_activeGameObject = gameObject;
}

void InspectorWindow::ShowMeshRenderComponent()
{
	ImGui::Separator();
	static char meshBuffer[128];
	bool isHeaderOpen = ImGui::CollapsingHeader("Mesh Render Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X")) {
		// remove component
		_scene.RemoveComponent(*_activeGameObject->GetComponent<RenderComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Mesh");
		ImGui::SameLine();
		ImGui::InputText("##Mesh Name",meshBuffer,64);
		ImGui::SameLine();
		if(ImGui::Button("Choose mesh"))
		{
			// TODO: When file manager added, use file manager to select a mesh
			ImGuiFileDialog::Instance()->OpenDialog("getFileName","Choose Mesh",".obj",".");
		}
	}

	if(ImGuiFileDialog::Instance()->Display("getFileName", ImGuiWindowFlags_NoCollapse, FileDialog::MIN_SIZE, FileDialog::MAX_SIZE))
	{
		if(ImGuiFileDialog::Instance()->IsOk())
		{
			_meshFileName = ImGuiFileDialog ::Instance()->GetCurrentFileName();
		}
		ImGuiFileDialog::Instance()->Close();
	}
	strncpy(meshBuffer, _meshFileName.c_str(), sizeof(meshBuffer));
}

void InspectorWindow::ShowTransformComponent()
{
	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X")) {
		// remove component
		_scene.RemoveComponent(*_activeGameObject->GetComponent<TransformComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(80);
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::InputFloat("##Xpos", &_activeGameObject->GetComponent<TransformComponent>()->position[0]);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::InputFloat("##Ypos", &_activeGameObject->GetComponent<TransformComponent>()->position[1]);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::InputFloat("##Zpos", &_activeGameObject->GetComponent<TransformComponent>()->position[2]);

		static float eulerRotation[3] = {0.0f, 0.0f, 0.0f};
		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::InputFloat("##Xrot", &eulerRotation[0]);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::InputFloat("##Yrot", &eulerRotation[1]);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::InputFloat("##Zrpt", &eulerRotation[2]);
		_activeGameObject->GetComponent<TransformComponent>()->rotation = Maths::Quaternion::EulerToQuat(
				Maths::Vec3(eulerRotation[0], eulerRotation[1], eulerRotation[2]));

		ImGui::Text("Scale:    ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::InputFloat("##Xscale", &_activeGameObject->GetComponent<TransformComponent>()->scale[0]);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::InputFloat("##Yscale", &_activeGameObject->GetComponent<TransformComponent>()->scale[1]);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::InputFloat("##Zscale", &_activeGameObject->GetComponent<TransformComponent>()->scale[2]);
		ImGui::PopItemWidth();
	}
}

void InspectorWindow::ShowAddComponent()
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
				_scene.AddComponent<RenderComponent>(_activeGameObject);
			}
			else if (strcmp(selectedComponent, "Transform Component") == 0)
			{
				// Add Transform Component
				_scene.AddComponent<TransformComponent>(_activeGameObject);
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
		ImGui::EndChild();
	}
}
