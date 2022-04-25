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

		  if (_activeGameObject->GetComponent<CameraComponent>())
			  ShowCameraComponent(scene);

		  if (_activeGameObject->GetComponent<AudioComponent>())
			  ShowAudioComponent(scene);

		  ImGui::Separator();
		  if (_isAddComponentWindowOpen)
			  ShowAddComponent(scene);

			if (ImGui::Button("Add Component"))
			{
				_isAddComponentWindowOpen = !_isAddComponentWindowOpen;
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
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_TABLE_CELLS "  Mesh", ImGuiTreeNodeFlags_AllowItemOverlap);
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

		auto asset_Helper = _assetHelper->ShowGuiWindow();
		if(std::get<0>(asset_Helper))
		{
			_activeGameObject->GetComponent<RenderComponent>()->SetMesh(std::get<1>(asset_Helper));
		}
	}
}

void InspectorWindow::ShowTransformComponent(Scene& scene)
{
	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT "  Transform", ImGuiTreeNodeFlags_AllowItemOverlap);
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
		ImGui::InputFloat("##Xpos", &_activeGameObject->GetComponent<TransformComponent>()->localPosition[0]);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::InputFloat("##Ypos", &_activeGameObject->GetComponent<TransformComponent>()->localPosition[1]);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::InputFloat("##Zpos", &_activeGameObject->GetComponent<TransformComponent>()->localPosition[2]);

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
		_activeGameObject->GetComponent<TransformComponent>()->localRotation = Maths::Quaternion::EulerToQuaternion(
				Maths::Vec3(eulerRotation[0], eulerRotation[1], eulerRotation[2]));

		ImGui::Text("Scale:    ");
		ImGui::SameLine();
		ImGui::InputFloat("##scale", &_activeGameObject->GetComponent<TransformComponent>()->localScale);
		ImGui::PopItemWidth();
	}
}

void InspectorWindow::ShowCameraComponent(Scene& scene)
{
	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAMERA "  Camera", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveCameraComponent")) {
		// remove component
		scene.RemoveComponent(*_activeGameObject->GetComponent<CameraComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		auto camera = _activeGameObject->GetComponent<CameraComponent>();

//		IMGUI_API bool          BeginListBox(const char* label, const ImVec2& size = ImVec2(0, 0)); // open a framed scrolling region
//		IMGUI_API void          EndListBox();                                                       // only call EndListBox() if BeginListBox() returned true!
//		IMGUI_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
//		IMGUI_API bool          ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);

		const char* projectionTypes[] = { "perspective", "orthographic" };
		ImGui::ListBox(
				"Projection Type",
				(int*)(&camera->projectionType),
				projectionTypes,
				IM_ARRAYSIZE(projectionTypes));

		const char* clearModes[] = {"none", "backgroundColor","skybox" };
		ImGui::ListBox(
				"Clear Mode",
				(int*)(&camera->clearMode),
				clearModes,
				IM_ARRAYSIZE(clearModes));

		ImGui::PushItemWidth(50);
		ImGui::Text("Background color: ");
		ImGui::SameLine();
		ImGui::Text("R");
		ImGui::SameLine();
		ImGui::InputFloat("##Red", &camera->backgroundColor.r);
		ImGui::SameLine();
		ImGui::Text("G");
		ImGui::SameLine();
		ImGui::InputFloat("##Green", &camera->backgroundColor.g);
		ImGui::SameLine();
		ImGui::Text("B");
		ImGui::SameLine();
		ImGui::InputFloat("##Blue", &camera->backgroundColor.b);
		ImGui::SameLine();
		ImGui::Text("A");
		ImGui::SameLine();
		ImGui::InputFloat("##Alpha", &camera->backgroundColor.a);

		float fov;
		float nearClippingPlane;
		float farClippingPlane;
		float orthographicSize;

		ImGui::Text("Field of View: ");
		ImGui::SameLine();
		ImGui::InputFloat("##FOV", &camera->fov);

		ImGui::Text("Near Clipping Plane: ");
		ImGui::SameLine();
		ImGui::InputFloat("##NearClippingPlane", &camera->nearClippingPlane);

		ImGui::Text("Far Clipping Plane: ");
		ImGui::SameLine();
		ImGui::InputFloat("##FarClippingPlane", &camera->farClippingPlane);

		ImGui::Text("Orthographic Size: ");
		ImGui::SameLine();
		ImGui::InputFloat("##OrthographicSize", &camera->orthographicSize);
	}
}

void InspectorWindow::ShowAudioComponent(Scene& scene)
{
	ImGui::Separator();
	char sampleBuffer[64];
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_TABLE_CELLS "  Audio", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRenderComponent")) {
		// remove component
		scene.RemoveComponent(*_activeGameObject->GetComponent<AudioComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Audio");
		ImGui::SameLine();
		ImGui::PushItemWidth(130.0f);

		// store the current mesh name into mesh buffer, so that we can display it in the input text box
		if(_activeGameObject->GetComponent<AudioComponent>() != nullptr)
			strcpy(sampleBuffer,  _activeGameObject->GetComponent<AudioComponent>()->fileName.c_str());
		else
			memset(sampleBuffer, 0, 64 * sizeof(char));

		// show text box (read only)
		ImGui::InputText("##Sample Name",sampleBuffer,sizeof(sampleBuffer), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if(ImGui::Button("Choose sample"))
		{
			ImGui::OpenPopup("Select Sample");
		}

		if(ImGui::Button("Play"))
		{
			_activeGameObject->GetComponent<AudioComponent>()->Play();
		}

		auto asset_Helper = _assetHelper->ShowAudioGuiWindow();
		if(std::get<0>(asset_Helper))
		{
			_activeGameObject->GetComponent<AudioComponent>()->LoadSample(std::get<1>(asset_Helper));
		}
	}
}

void InspectorWindow::ShowAddComponent(Scene& scene)
{
	if (ImGui::BeginChild("ComponentSelector"))
	{
		const char* components[] = {
				"Mesh Render Component",
				"Transform Component",
				"Camera Component",
				"Audio Component"
		};
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
			else if (strcmp(selectedComponent, "Camera Component") == 0)
			{
				scene.AddComponent<CameraComponent>(_activeGameObject);
			}
			else if (strcmp(selectedComponent, "Audio Component") == 0)
			{
				scene.AddComponent<AudioComponent>(_activeGameObject);
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
		ImGui::EndChild();
	}
}
