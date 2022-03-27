//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>

using namespace PlatinumEngine;

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
		if (_activeGameObject->GetComponent<MeshComponent>()!= nullptr)
			ShowMeshComponent();

		if (_activeGameObject->GetComponent<TransformComponent>()!= nullptr)
			ShowTransformComponent();
	}
	ImGui::End();
}

void InspectorWindow::SetActiveGameObject(GameObject* gameObject)
{
	_activeGameObject = gameObject;
}

void InspectorWindow::ShowMeshComponent()
{
	ImGui::Separator();
	static char meshBuffer[128];
	if (ImGui::CollapsingHeader("Mesh Render Component"))
	{
		ImGui::Text("Mesh");
		ImGui::SameLine();
		ImGui::InputText("##Mesh Name",meshBuffer,64);
		ImGui::SameLine();
		if(ImGui::Button("Choose mesh"))
		{
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
	if (ImGui::CollapsingHeader("Transform Component"))
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

		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::InputFloat("##Xrot", &_activeGameObject->GetComponent<TransformComponent>()->rotation[0]);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::InputFloat("##Yrot", &_activeGameObject->GetComponent<TransformComponent>()->rotation[1]);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::InputFloat("##Zrpt", &_activeGameObject->GetComponent<TransformComponent>()->rotation[2]);

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
