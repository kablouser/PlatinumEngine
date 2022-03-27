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
<<<<<<< HEAD
=======
		// Name box
>>>>>>> origin/inspector_window
		ImGui::Text("Object Name: ");
		ImGui::SameLine();
		static char buffer[128];
		strcpy(buffer, _activeGameObject->name.c_str());
<<<<<<< HEAD
		ImGui::InputText(" ", buffer, IM_ARRAYSIZE(buffer));
		_activeGameObject->name = std::string{buffer};

		// Now render each component gui
		if (_activeGameObject->HasComponent<MeshComponent>())
			ShowMeshComponent();
		if (_activeGameObject->HasComponent<TransformComponent>())
			ShowTransformComponent();
	}
=======
		ImGui::InputText("##Object Name", buffer, IM_ARRAYSIZE(buffer));
		_activeGameObject->name = std::string{buffer};

		// Now render each component gui
		if (_activeGameObject->GetComponent<MeshComponent>())
			ShowMeshComponent();
		if (_activeGameObject->GetComponent<TransformComponent>())
			ShowTransformComponent();
	}

>>>>>>> origin/inspector_window
	ImGui::End();
}

void InspectorWindow::SetActiveGameObject(GameObject* gameObject)
{
	_activeGameObject = gameObject;
}

void InspectorWindow::ShowMeshComponent()
{
<<<<<<< HEAD
=======
	// TODO: Once mesh renderer component made
>>>>>>> origin/inspector_window
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Mesh Component"))
	{
		ImGui::Text("I am a mesh");
	}
}

void InspectorWindow::ShowTransformComponent()
{
<<<<<<< HEAD
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
=======
	// TODO: Check does this work with transform component when made?
	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Transform Component"))
	{
		ImGui::PushItemWidth(75);
>>>>>>> origin/inspector_window
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
<<<<<<< HEAD
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
=======
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
>>>>>>> origin/inspector_window
		ImGui::PopItemWidth();
	}
}
