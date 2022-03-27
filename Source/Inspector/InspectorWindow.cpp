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
		ImGui::Text("Object Name: ");
		ImGui::SameLine();
		static char buffer[128];
		strcpy(buffer, _activeGameObject->name.c_str());
		ImGui::InputText(" ", buffer, IM_ARRAYSIZE(buffer));
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
	if (ImGui::CollapsingHeader("Mesh Component"))
	{
		ImGui::Text("I am a mesh");
	}
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
		ImGui::PushItemWidth(50);
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::InputFloat(" ", &position[0]);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::InputFloat(" ", &position[1]);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::InputFloat(" ", &position[2]);
		std::copy(std::begin(position),
				std::end(position),
				std::begin(_activeGameObject->GetComponent<TransformComponent>()->position));

//		ImGui::Text("Rotation: ");
//		ImGui::SameLine();
//		ImGui::InputFloat3(" ", rotation);
//
//		ImGui::Text("Scale: ");
//		ImGui::SameLine();
//		ImGui::InputFloat3(" ", scale);
		ImGui::PopItemWidth();
	}
}
