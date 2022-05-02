//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>
#include <ComponentComposition/CameraComponent.h>
#include <ImGuizmo.h>
#include <SceneManager/Scene.h>

using namespace PlatinumEngine;

InspectorWindow::InspectorWindow(AssetHelper* assetHelper, SceneEditor* sceneEditor) :
	_assetHelper(assetHelper), _sceneEditor(sceneEditor) {}

void InspectorWindow::ShowGUIWindow(bool* isOpen, Scene& scene)
{
	if(ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector Window", isOpen))
	{
		auto& obj = _sceneEditor->GetSelectedGameobject();
		if (obj)
		{
			ImGui::Text("Name: ");
			ImGui::SameLine();
			static char objectNameBuffer[128];
			strcpy(objectNameBuffer, obj.DeRef()->name.c_str());
			ImGui::InputText("##input name", objectNameBuffer, IM_ARRAYSIZE(objectNameBuffer));
			obj.DeRef()->name = std::string( objectNameBuffer );

			ImGui::SameLine();
			bool isEnabled = obj.DeRef()->IsEnabled();

			if (ImGui::Checkbox("##IsEnabled", &isEnabled))
			{
				obj.DeRef()->SetEnabled(isEnabled, scene);
			}

			// Now render each component gui
			if (obj.DeRef()->GetComponent<RenderComponent>())
				ShowMeshRenderComponent(scene);

			if (obj.DeRef()->GetComponent<TransformComponent>())
				ShowTransformComponent(scene);

		  	if (obj.DeRef()->GetComponent<CameraComponent>())
				  ShowCameraComponent(scene);

		  	ImGui::Separator();
		  	if (_isAddComponentWindowOpen)
				  ShowAddComponent(scene);
		  	else
		  	{
				  if (ImGui::Button("Add Component"))
				  {
					  _isAddComponentWindowOpen = !_isAddComponentWindowOpen;
				  }

				  ImGui::SameLine();
				  if (ImGui::Button("Remove Object"))
				  {
						_sceneEditor->DeleteSelectedGameObject();
				  }
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

void InspectorWindow::ShowMeshRenderComponent(Scene& scene)
{
	ImGui::Separator();
	std::string meshBuffer, textureBuffer, normalTextureBuffer;
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_VECTOR_SQUARE "  Mesh Render Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	SavedReference<GameObject>& obj = _sceneEditor->GetSelectedGameobject();
	SavedReference<RenderComponent> renderComponent = obj.DeRef()->GetComponent<RenderComponent>();

	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRenderComponent")) {
		// remove component
		scene.RemoveComponent(renderComponent);
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Mesh Properties");
		ImGui::Separator();
		ImGui::Text("File");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);

		// store the current mesh name into mesh buffer, so that we can display it in the input text box
		if(renderComponent.DeRef()->GetMesh())
			meshBuffer = renderComponent.DeRef()->GetMesh().DeRef()->fileName;

		// show text box (read only)--------Choose Mesh
		ImGui::InputText("##Mesh Name",&meshBuffer[0],meshBuffer.size(), ImGuiInputTextFlags_ReadOnly);
		//Enables DragDrop for TextBox
		if (ImGui::BeginDragDropTarget())
		{
			//Accept any regular file (but it will check if it is mesh or not)
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
					std::cout<<"NAME: "<<payloadPath.filename().string()<<"\n";
					//Maybe we SetMesh on _activeGameObject
					//_activeGameObject->GetComponent<RenderComponent>()->SetMesh(mesh);
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if(ImGui::Button("Select Mesh"))
		{
			ImGui::OpenPopup("Select Mesh");
		}

		{
			auto[isClicked, assetReference] = _assetHelper->PickAssetGUIWindow<Mesh>("Select Mesh");
			if (isClicked)
				renderComponent.DeRef()->SetMesh(assetReference);
		}

		if(renderComponent.DeRef()->material.diffuseTexture)
			textureBuffer = renderComponent.DeRef()->material.diffuseTexture.DeRef()->fileName;

		if(renderComponent.DeRef()->material.normalTexture)
			normalTextureBuffer = renderComponent.DeRef()->material.normalTexture.DeRef()->fileName;

		//Show text box (read only)----------Choose Material
		ImGui::Text("%s", "Material Properties");
		ImGui::Separator();

		ImGui::Text("Shininess");

		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::SliderFloat("##shininess",&(renderComponent.DeRef()->material.shininessFactor),0.f, 100.f, "%.3f", ImGuiSliderFlags_None);
		ImGui::PopItemWidth();

		ImGui::Text("Blinn-Phong");
		ImGui::SameLine();
		ImGui::Checkbox("##Blinn-Phong", &(renderComponent.DeRef()->material.useBlinnPhong));

		ImGui::Text("Texture");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);

		// store the current material name into mesh buffer, so that we can display it in the input text box

		ImGui::InputText("##Material Name", &textureBuffer[0], textureBuffer.size(), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::Button("Select##Texture"))
		{
			ImGui::OpenPopup("Select Texture");
		}
		{
			auto[isClicked, assetReference] = _assetHelper->PickAssetGUIWindow<Texture>("Select Texture");
			if (isClicked)
			{
				renderComponent.DeRef()->SetMaterial(assetReference);
				renderComponent.DeRef()->material.useTexture = true;
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseTexture", &(renderComponent.DeRef()->material.useTexture));

		ImGui::Text("%s", "Normal Map");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::InputText("##Normal Map Name", &normalTextureBuffer[0], normalTextureBuffer.size(), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::Button("Select##NormalTexture"))
		{
			ImGui::OpenPopup("Select Normal Texture");
		}
		{
			auto[isClicked, assetReference] = _assetHelper->PickAssetGUIWindow<Texture>("Select Normal Texture");
			if (isClicked)
				renderComponent.DeRef()->SetNormalMap(assetReference);
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseNormalTexture", &(renderComponent.DeRef()->material.useNormalTexture));
	}
}

void InspectorWindow::ShowTransformComponent(Scene& scene)
{
	auto& obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_ARROWS_TURN_TO_DOTS "  Transform Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveTransformComponent"))
	{
		// remove component
		scene.RemoveComponent(obj.DeRef()->GetComponent<TransformComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Position: ");
		ImGui::SameLine(_textWidthTransformComponent);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xpos", &obj.DeRef()->GetComponent<TransformComponent>().DeRef()->localPosition[0], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ypos", &obj.DeRef()->GetComponent<TransformComponent>().DeRef()->localPosition[1], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zpos", &obj.DeRef()->GetComponent<TransformComponent>().DeRef()->localPosition[2], 0.001f);
    
    	Maths::Vec3 eulerRotation = obj.DeRef()->GetComponent<TransformComponent>().DeRef()->localRotation.EulerAngles();
		ImGui::Text(ICON_FA_ROTATE " Rotation: ");
		ImGui::SameLine(_textWidthTransformComponent);
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
		obj.DeRef()->GetComponent<TransformComponent>().DeRef()->localRotation = Maths::Quaternion::EulerToQuaternion(
				Maths::Vec3(eulerRotation[0], eulerRotation[1], eulerRotation[2])); 

		ImGui::Text(ICON_FA_MAXIMIZE " Scale: ");
		// Scale is special case and needs some extra offset applied
		ImGui::SameLine(_textWidthTransformComponent + 16.0f);
		ImGui::InputFloat("##scale", &obj.DeRef()->GetComponent<TransformComponent>().DeRef()->localScale);
		ImGui::PopItemWidth();
	}

}

void InspectorWindow::ShowCameraComponent(Scene& scene)
{
	char cameraType[64];
	char clearMode[64];
	auto& obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAMERA "  Camera Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveCameraComponent"))
	{
		// remove component
		scene.RemoveComponent(obj.DeRef()->GetComponent<CameraComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		auto camera = obj.DeRef()->GetComponent<CameraComponent>();

		{// the camera projection type
			if (CameraComponent::ProjectionType::perspective == camera.DeRef()->projectionType)
			{
				std::strcpy(cameraType, _temp[0].c_str());
			}
			if (CameraComponent::ProjectionType::orthographic == camera.DeRef()->projectionType)
			{

				std::strcpy(cameraType, _temp[1].c_str());
      }

			ImGui::Text("Projection Type");
			ImGui::SameLine(_textWidthCameraComponent);
			ImGui::SetNextItemWidth(_itemWidthMeshRenderComponent);
			ImGui::InputText("##Projection Type", cameraType, sizeof(cameraType), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::BeginPopupContextItem("projection type"))
			{
				if (ImGui::Selectable("Perspective"))
				{
					camera.DeRef()->projectionType = CameraComponent::ProjectionType::perspective;
					//std::strcpy(cameraType, _temp[0].c_str());
				}
				if (ImGui::Selectable("Orthographic"))
				{
					camera.DeRef()->projectionType = CameraComponent::ProjectionType::orthographic;
					//std::strcpy(cameraType, _temp[1].c_str());
				}
				ImGui::EndPopup();
			}

			if (ImGui::Button(ICON_FA_CARET_DOWN "##projectionType"))
			{
				ImGui::OpenPopup("projection type");
			}
		}

		{// the camera clear mode
			if (CameraComponent::ClearMode::backgroundColor == camera.DeRef()->clearMode)
			{
				std::strcpy(clearMode, _clearMode[2].c_str());
			}
			if (CameraComponent::ClearMode::skybox == camera.DeRef()->clearMode)
			{
				std::strcpy(clearMode, _clearMode[1].c_str());
			}
			if (CameraComponent::ClearMode::none == camera.DeRef()->clearMode)
			{
				std::strcpy(clearMode, _clearMode[0].c_str());
			}


			ImGui::Text("Clear Mode");
			ImGui::SameLine(_textWidthCameraComponent);
			ImGui::SetNextItemWidth(_itemWidthMeshRenderComponent);
			ImGui::InputText("##Clear Mode", clearMode, sizeof(clearMode), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::BeginPopupContextItem("clear mode"))
			{
				if (ImGui::Selectable("SkyBox"))
				{
					camera.DeRef()->clearMode = CameraComponent::ClearMode::skybox;
				}
				if (ImGui::Selectable("None"))
				{
					camera.DeRef()->clearMode = CameraComponent::ClearMode::none;
				}
				if (ImGui::Selectable("BackgroundColour"))
				{
					camera.DeRef()->clearMode = CameraComponent::ClearMode::backgroundColor;
				}
				ImGui::EndPopup();
			}

			if (ImGui::Button(ICON_FA_CARET_DOWN "##ClearMode"))
			{
				ImGui::OpenPopup("clear mode");
			}
		}
		ImGui::Separator();
		ImGui::PushItemWidth(50);
		ImGui::Text("Background color: ");

		// To avoid making window excessively wide, start new line
		ImGui::Text("R");
		ImGui::SameLine();
		ImGui::InputFloat("##Red", &camera.DeRef()->backgroundColor.r);
		ImGui::SameLine();
    
		ImGui::Text("G");
		ImGui::SameLine();
		ImGui::InputFloat("##Green", &camera.DeRef()->backgroundColor.g);
		ImGui::SameLine();
    
		ImGui::Text("B");
		ImGui::SameLine();
		ImGui::InputFloat("##Blue", &camera.DeRef()->backgroundColor.b);
		ImGui::SameLine();
    
		ImGui::Text("A");
		ImGui::SameLine();
		ImGui::InputFloat("##Alpha", &camera.DeRef()->backgroundColor.a);

		float fov;
		float nearClippingPlane;
		float farClippingPlane;
		float orthographicSize;

		ImGui::Text("Field of View: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##FOV", &camera.DeRef()->fov);

		ImGui::Text("Near Clipping Plane: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##NearClippingPlane", &camera.DeRef()->nearClippingPlane);

		ImGui::Text("Far Clipping Plane: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##FarClippingPlane", &camera.DeRef()->farClippingPlane);

		ImGui::Text("Orthographic Size: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##OrthographicSize", &camera.DeRef()->orthographicSize);
	}
}

void InspectorWindow::ShowAddComponent(Scene& scene)
{
	if (ImGui::BeginChild("ComponentSelector"))
	{
		auto& obj = _sceneEditor->GetSelectedGameobject();

		const char* components[] = {
				"Mesh Render Component",
				"Transform Component",
				"Camera Component"
		};
		static const char* selectedComponent = nullptr;
		static char componentSelectorBuffer[128];
		ImGui::Text("%s", "Select a Component");
		ImGui::InputText("##FilterComponents", componentSelectorBuffer, IM_ARRAYSIZE(componentSelectorBuffer));
		ImGui::SameLine();
		ImGui::Text("%s", ICON_FA_MAGNIFYING_GLASS);

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
				scene.AddComponent<RenderComponent>(obj);
			}
			else if (strcmp(selectedComponent, "Transform Component") == 0)
			{
				// Add Transform Component
				scene.AddComponent<TransformComponent>(obj);
			}
			else if (strcmp(selectedComponent, "Camera Component") == 0)
			{
				scene.AddComponent<CameraComponent>(obj);
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
		ImGui::EndChild();
	}
}