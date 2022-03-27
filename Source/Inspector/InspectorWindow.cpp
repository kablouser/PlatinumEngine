//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>

using namespace PlatinumEngine;

static char buf1[64] = "";
void InspectorWindow::ShowGUIWindow(bool* isOpen)
{
	ImGui::Begin("Inspector Window", isOpen);

	if (_activeGameObject)
	{
		ImGui::Text("Object Name: ");
		ImGui::SameLine();
		static char buffer[128];
		strcpy(buffer, _activeGameObject->name.c_str());
		ImGui::InputText("##input name", buffer, IM_ARRAYSIZE(buffer));
		_activeGameObject->name = std::string{buffer};

		// Now render each component gui
		if (_activeGameObject->HasComponent<MeshComponent>())
			ShowMeshComponent();

		if (_activeGameObject->HasComponent<TransformComponent>())
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
	if (ImGui::CollapsingHeader("Mesh Render Component"))
	{
		ImGui::Text("Mesh");
		ImGui::SameLine();
		ImGui::InputText("##Mesh Name",buf1,64);
		ImGui::SameLine();
		if(ImGui::Button("choose mesh"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("getFileName","Choose Mesh",".obj",".");
		}
	}
	if(ImGuiFileDialog::Instance()->Display("getFileName", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
	{
		if(ImGuiFileDialog::Instance()->IsOk())
		{

			fileName = ImGuiFileDialog ::Instance()->GetCurrentFileName();
		}
		ImGuiFileDialog::Instance()->Close();
	}
	strncpy(buf1, fileName.c_str(), sizeof(buf1));
}

void InspectorWindow::ShowTransformComponent()
{
	// If this gui is being shown, assumption that object has transform component
	static float position[3];
	std::copy(std::begin(_activeGameObject->GetComponent<TransformComponent>()->position),
			std::end(_activeGameObject->GetComponent<TransformComponent>()->position),
			std::begin(position));
	static float rotation[3];
	std::copy(std::begin(_activeGameObject->GetComponent<TransformComponent>()->rotation),
			std::end(_activeGameObject->GetComponent<TransformComponent>()->rotation),
			std::begin(rotation));
	static float scale[3];
	std::copy(std::begin(_activeGameObject->GetComponent<TransformComponent>()->scale),
			std::end(_activeGameObject->GetComponent<TransformComponent>()->scale),
			std::begin(scale));
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Transform Component"))
	{
		ImGui::PushItemWidth(80);
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::InputFloat("##Xpos", &position[0]);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::InputFloat("##Ypos", &position[1]);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::InputFloat("##Zpos", &position[2]);
		std::copy(std::begin(position),
				std::end(position),
				std::begin(_activeGameObject->GetComponent<TransformComponent>()->position));

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
