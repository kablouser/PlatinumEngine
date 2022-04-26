//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>
#include <ComponentComposition/CameraComponent.h>
#include <ImGuizmo.h>

using namespace PlatinumEngine;

InspectorWindow::InspectorWindow(AssetHelper* assetHelper, SceneEditor* sceneEditor) :
	_assetHelper(assetHelper), _sceneEditor(sceneEditor) {}

void InspectorWindow::ShowGUIWindow(bool* isOpen, Scene& scene)
{
	if(ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector Window", isOpen))
	{
		auto obj = _sceneEditor->GetSelectedGameobject();
		if (obj != nullptr)
		{
			ImGui::Text("Name: ");
			ImGui::SameLine();
			static char objectNameBuffer[128];
			strcpy(objectNameBuffer, obj->name.c_str());
			ImGui::InputText("##input name", objectNameBuffer, IM_ARRAYSIZE(objectNameBuffer));
			obj->name = std::string( objectNameBuffer );

			ImGui::SameLine();
			bool isEnabled = obj->IsEnabled();

			if (ImGui::Checkbox("##IsEnabled", &isEnabled))
			{
				obj->SetEnabled(isEnabled, scene);
			}

			// Now render each component gui
			if (obj->GetComponent<RenderComponent>() != nullptr)
				ShowMeshRenderComponent(scene);

			if (obj->GetComponent<TransformComponent>() != nullptr)
				ShowTransformComponent(scene);

		  	if (obj->GetComponent<CameraComponent>())
				ShowCameraComponent(scene);

		  	if (obj->GetComponent<ParticleEffect>() != nullptr)
				ShowParticleEffectComponent(scene);

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
	char meshBuffer[64];
	char textureBuffer[64];
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_VECTOR_SQUARE "  Mesh Render Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	char normalTextureBuffer[64];
	auto obj = _sceneEditor->GetSelectedGameobject();

	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRenderComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<RenderComponent>());
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
		if(obj->GetComponent<RenderComponent>()->GetMesh() != nullptr)
			strcpy(meshBuffer,  obj->GetComponent<RenderComponent>()->GetMesh()->fileName.c_str());
		else
			memset(meshBuffer, 0, 64 * sizeof(char));

		// show text box (read only)--------Choose Mesh
		ImGui::InputText("##Mesh Name",meshBuffer,sizeof(meshBuffer), ImGuiInputTextFlags_ReadOnly);
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
			auto asset_Helper = _assetHelper->ShowMeshGuiWindow();
			if (std::get<0>(asset_Helper))
			{
				obj->GetComponent<RenderComponent>()->SetMesh(std::get<1>(asset_Helper));
			}
		}

		if(obj->GetComponent<RenderComponent>()->material.diffuseTexture != nullptr)
			strcpy(textureBuffer,  obj->GetComponent<RenderComponent>()->material.diffuseTexture->fileName.c_str());
		else
			memset(textureBuffer, 0, 64 * sizeof(char));

		if(obj->GetComponent<RenderComponent>()->material.normalTexture != nullptr)
			strcpy(normalTextureBuffer,  obj->GetComponent<RenderComponent>()->material.normalTexture->fileName.c_str());
		else
			memset(normalTextureBuffer, 0, 64 * sizeof(char));

		//Show text box (read only)----------Choose Material
		ImGui::Text("%s", "Material Properties");
		ImGui::Separator();

		ImGui::Text("Specular Exponent");

		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::SliderFloat("##shininess",&(obj->GetComponent<RenderComponent>()->material.shininessFactor),0.f, 100.f, "%.3f", ImGuiSliderFlags_None);
		ImGui::PopItemWidth();

		ImGui::Text("Blinn-Phong");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::Checkbox("##Blinn-Phong", &(obj->GetComponent<RenderComponent>()->material.useBlinnPhong));

		ImGui::Text("Texture");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);

		// store the current material name into mesh buffer, so that we can display it in the input text box

		ImGui::InputText("##Material Name", textureBuffer, sizeof(textureBuffer), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::Button("Select##Texture"))
		{
			ImGui::OpenPopup("Select Texture");
		}
		{
			auto asset_Helper = _assetHelper->ShowTextureGuiWindow();
			if (std::get<0>(asset_Helper))
			{
				obj->GetComponent<RenderComponent>()->SetMaterial(std::get<1>(asset_Helper));
				obj->GetComponent<RenderComponent>()->material.useTexture = true;
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseTexture", &(obj->GetComponent<RenderComponent>()->material.useTexture));

		ImGui::Text("%s", "Normal Map");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::InputText("##Normal Map Name", normalTextureBuffer, sizeof(normalTextureBuffer), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::Button("Select##NormalTexture"))
		{
			ImGui::OpenPopup("Select Normal Texture");
		}
		{
			auto asset_Helper = _assetHelper->ShowNormalTextureGuiWindow();
			if (std::get<0>(asset_Helper))
				obj->GetComponent<RenderComponent>()->SetNormalMap(std::get<1>(asset_Helper));
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseNormalTexture", &(obj->GetComponent<RenderComponent>()->material.useNormalTexture));

		ImGui::Text("Special Properties");
		ImGui::Separator();
		ImGui::Text("Reflection");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::Checkbox("##UseRelfectionShader", &(obj->GetComponent<RenderComponent>()->material.useReflectionShader));
		ImGui::Text("Refraction");
		ImGui::SameLine(_textWidthMeshRenderComponent);
		ImGui::Checkbox("##UseRefracctionShader", &(obj->GetComponent<RenderComponent>()->material.useRefractionShader));
		ImGui::Text("Refraction Index");
		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		// Setting max to 4 as you start to get unpleasing results past that
		ImGui::SliderFloat("##refractionIndex",&(obj->GetComponent<RenderComponent>()->material.refractionIndex),1.0f, 4.f, "%.3f", ImGuiSliderFlags_None);
		ImGui::PopItemWidth();
	}
}

void InspectorWindow::ShowTransformComponent(Scene& scene)
{
	auto obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_ARROWS_TURN_TO_DOTS "  Transform Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveTransformComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<TransformComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Position: ");
		ImGui::SameLine(_textWidthTransformComponent);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xpos", &obj->GetComponent<TransformComponent>()->localPosition[0], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ypos", &obj->GetComponent<TransformComponent>()->localPosition[1], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zpos", &obj->GetComponent<TransformComponent>()->localPosition[2], 0.001f);
    
    	Maths::Vec3 eulerRotation = obj->GetComponent<TransformComponent>()->localRotation.EulerAngles();
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
		obj->GetComponent<TransformComponent>()->localRotation = Maths::Quaternion::EulerToQuaternion(
				Maths::Vec3(eulerRotation[0], eulerRotation[1], eulerRotation[2])); 

		ImGui::Text(ICON_FA_MAXIMIZE " Scale: ");
		// Scale is special case and needs some extra offset applied
		ImGui::SameLine(_textWidthTransformComponent + 16.0f);
		ImGui::InputFloat("##scale", &obj->GetComponent<TransformComponent>()->localScale);
		ImGui::PopItemWidth();
	}

}

void InspectorWindow::ShowCameraComponent(Scene& scene)
{
	char cameraType[64];
	char clearMode[64];
	auto obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAMERA "  Camera Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveCameraComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<CameraComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		auto camera = obj->GetComponent<CameraComponent>();

		{// the camera projection type
			if (CameraComponent::ProjectionType::perspective == camera->projectionType)
			{
				std::strcpy(cameraType, _temp[0].c_str());
			}
			if (CameraComponent::ProjectionType::orthographic == camera->projectionType)
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
					camera->projectionType = CameraComponent::ProjectionType::perspective;
					//std::strcpy(cameraType, _temp[0].c_str());
				}
				if (ImGui::Selectable("Orthographic"))
				{
					camera->projectionType = CameraComponent::ProjectionType::orthographic;
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
			if (CameraComponent::ClearMode::backgroundColor == camera->clearMode)
			{
				std::strcpy(clearMode, _clearMode[2].c_str());
			}
			if (CameraComponent::ClearMode::skybox == camera->clearMode)
			{
				std::strcpy(clearMode, _clearMode[1].c_str());
			}
			if (CameraComponent::ClearMode::none == camera->clearMode)
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
					camera->clearMode = CameraComponent::ClearMode::skybox;
				}
				if (ImGui::Selectable("None"))
				{
					camera->clearMode = CameraComponent::ClearMode::none;
				}
				if (ImGui::Selectable("BackgroundColour"))
				{
					camera->clearMode = CameraComponent::ClearMode::backgroundColor;
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
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##FOV", &camera->fov);

		ImGui::Text("Near Clipping Plane: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##NearClippingPlane", &camera->nearClippingPlane);

		ImGui::Text("Far Clipping Plane: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##FarClippingPlane", &camera->farClippingPlane);

		ImGui::Text("Orthographic Size: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##OrthographicSize", &camera->orthographicSize);
	}
}

void InspectorWindow::ShowParticleEffectComponent(Scene &scene)
{
	auto obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_FIRE "  Particle Effect Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveParticleEffectComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<ParticleEffect>());
		return;
	}

	if (isHeaderOpen)
	{

	}
}

void InspectorWindow::ShowAddComponent(Scene& scene)
{
	if (ImGui::BeginChild("ComponentSelector"))
	{
		auto obj = _sceneEditor->GetSelectedGameobject();

		const char* components[] = {
				"Mesh Render Component",
				"Transform Component",
				"Camera Component",
				"Particle Effect Component"
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
			else if (strcmp(selectedComponent, "Particle Effect Component") == 0)
			{
				// Add Particle Effect Component
				scene.AddComponent<ParticleEffect>(obj);
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
		ImGui::EndChild();
	}
}