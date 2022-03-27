//
// Created by Matt on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>

using namespace PlatinumEngine;

void InspectorWindow::ShowGUIWindow(bool* isOpen)
{
	ImGui::Begin("Inspector Window", isOpen);

	if (_activeGameObject)
	{
		// Name box
		ImGui::Text("Object Name: ");
		ImGui::SameLine();
		static char buffer[128];
		strcpy(buffer, _activeGameObject->name.c_str());
		ImGui::InputText("##Object Name", buffer, IM_ARRAYSIZE(buffer));
		_activeGameObject->name = std::string{buffer};

		// Now render each component gui
		if (_activeGameObject->GetComponent<MeshComponent>())
			ShowMeshComponent();
		if (_activeGameObject->GetComponent<TransformComponent>())
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
	// TODO: Once mesh renderer component made
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Mesh Component"))
	{
		ImGui::Text("I am a mesh");
	}
}

void InspectorWindow::ShowTransformComponent()
{
	// TODO: Check does this work with transform component when made?
	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Transform Component"))
	{
		ImGui::PushItemWidth(75);
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
