//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>
#include <ComponentComposition/Camera.h>

#include <ImGuizmo.h>
#include <Inspector/Bezier.h>



using namespace PlatinumEngine;

InspectorWindow::InspectorWindow(AssetHelper* assetHelper, SceneEditor* sceneEditor, Physics* physics) :
	_assetHelper(assetHelper), _sceneEditor(sceneEditor), _physics(physics) {}

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
			if (obj->GetComponent<MeshRender>() != nullptr)
				ShowMeshRenderComponent(scene);

			if (obj->GetComponent<Transform>() != nullptr)
				ShowTransformComponent(scene);

			if (obj->GetComponent<Camera>() != nullptr)
			  	ShowCameraComponent(scene);

			if(obj->GetComponent<RigidBody>() != nullptr)
				ShowRigidBodyComponent(scene);

			if(obj->GetComponent<BoxCollider>() != nullptr)
				ShowBoxColliderComponent(scene);

			if(obj->GetComponent<SphereCollider>() != nullptr)
				ShowSphereColliderComponent(scene);

			if(obj->GetComponent<CapsuleCollider>() != nullptr)
				ShowCapsuleColliderComponent(scene);

		  	if (obj->GetComponent<ParticleEffect>() != nullptr)
				ShowParticleEffectComponent(scene);

		  if (obj->GetComponent<AudioComponent>() != nullptr)
			  ShowAudioComponent(scene);
      
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
		scene.RemoveComponent(*obj->GetComponent<MeshRender>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Mesh Properties");
		ImGui::Separator();
		ImGui::Text("File");
		ImGui::SameLine(_textWidth);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);

		// store the current mesh name into mesh buffer, so that we can display it in the input text box
		if(obj->GetComponent<MeshRender>()->GetMesh() != nullptr)
			strcpy(meshBuffer,  obj->GetComponent<MeshRender>()->GetMesh()->fileName.c_str());
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
				std::filesystem::path payloadPath = GetPayloadPath(payload);
				if(payloadPath.extension()==".obj")
				{
					//Maybe we SetMesh on obj
					//obj->GetComponent<MeshRender>()->SetMesh(mesh);
					//Set The mesh that we dragged to the RenderComponent
					auto asset_Helper = _assetHelper->GetMeshAsset(payloadPath.string());
					if (std::get<0>(asset_Helper))
						obj->GetComponent<MeshRender>()->SetMesh(std::get<1>(asset_Helper));
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if(ImGui::Button("Select"))
		{
			ImGui::OpenPopup("Select Mesh");
		}

		{
			auto asset_Helper = _assetHelper->ShowMeshGuiWindow();
			if (std::get<0>(asset_Helper))
			{
				obj->GetComponent<MeshRender>()->SetMesh(std::get<1>(asset_Helper));
			}
		}

		if(obj->GetComponent<MeshRender>()->material.diffuseTexture != nullptr)
			strcpy(textureBuffer,  obj->GetComponent<MeshRender>()->material.diffuseTexture->fileName.c_str());
		else
			memset(textureBuffer, 0, 64 * sizeof(char));

		if(obj->GetComponent<MeshRender>()->material.normalTexture != nullptr)
			strcpy(normalTextureBuffer,  obj->GetComponent<MeshRender>()->material.normalTexture->fileName.c_str());
		else
			memset(normalTextureBuffer, 0, 64 * sizeof(char));

		//Show text box (read only)----------Choose Material
		ImGui::Text("%s", "Material Properties");
		ImGui::Separator();

		ImGui::Text("Specular Exponent");
		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::SliderFloat("##shininess",&(obj->GetComponent<MeshRender>()->material.shininessFactor),0.f, 100.f, "%.3f", ImGuiSliderFlags_None);
		ImGui::PopItemWidth();

		ImGui::Text("Blinn-Phong");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##Blinn-Phong", &(obj->GetComponent<MeshRender>()->material.useBlinnPhong));

		ImGui::Text("Texture");
		ImGui::SameLine(_textWidth);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);

		// store the current material name into mesh buffer, so that we can display it in the input text box

		ImGui::InputText("##Material Name", textureBuffer, sizeof(textureBuffer), ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			//Accept any regular file (but it will check if it is texture or not)
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
			{
				std::filesystem::path payloadPath = GetPayloadPath(payload);
				if(payloadPath.extension()==".png")
				{
					//Set The texture that we dragged to the RenderComponent
					auto asset_Helper = _assetHelper->GetTextureAsset(payloadPath.string());
					if (std::get<0>(asset_Helper))
					{
						obj->GetComponent<MeshRender>()->SetMaterial(std::get<1>(asset_Helper));
						obj->GetComponent<MeshRender>()->material.useTexture = true;
					}
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
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
				obj->GetComponent<MeshRender>()->SetMaterial(std::get<1>(asset_Helper));
				obj->GetComponent<MeshRender>()->material.useTexture = true;
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseTexture", &(obj->GetComponent<MeshRender>()->material.useTexture));

		ImGui::Text("%s", "Normal Map");
		ImGui::SameLine(_textWidth);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::InputText("##Normal Map Name", normalTextureBuffer, sizeof(normalTextureBuffer), ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			//Accept any regular file (but it will check if it is texture or not) [Same things as before]
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
			{
				std::filesystem::path payloadPath = GetPayloadPath(payload);
				if(payloadPath.extension()==".png")
				{
					//Set The texture that we dragged to the RenderComponent
					auto asset_Helper = _assetHelper->GetTextureAsset(payloadPath.string());
					if (std::get<0>(asset_Helper))
						obj->GetComponent<MeshRender>()->SetNormalMap(std::get<1>(asset_Helper));
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::Button("Select##NormalTexture"))
		{
			ImGui::OpenPopup("Select Normal Texture");
		}
		{
			auto asset_Helper = _assetHelper->ShowNormalTextureGuiWindow();
			if (std::get<0>(asset_Helper))
				obj->GetComponent<MeshRender>()->SetNormalMap(std::get<1>(asset_Helper));
		}
		ImGui::SameLine();

		ImGui::Checkbox("##UseNormalTexture", &(obj->GetComponent<MeshRender>()->material.useNormalTexture));

		ImGui::Text("Special Properties");
		ImGui::Separator();
		ImGui::Text("Reflection");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##UseRelfectionShader", &(obj->GetComponent<MeshRender>()->material.useReflectionShader));
		ImGui::Text("Refraction");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##UseRefracctionShader", &(obj->GetComponent<MeshRender>()->material.useRefractionShader));
		ImGui::Text("Refraction Index");
		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		// Setting max to 4 as you start to get unpleasing results past that
		ImGui::SliderFloat("##refractionIndex",&(obj->GetComponent<MeshRender>()->material.refractionIndex),1.0f, 4.f, "%.3f", ImGuiSliderFlags_None);
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
		scene.RemoveComponent(*obj->GetComponent<Transform>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Position: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xpos", &obj->GetComponent<Transform>()->localPosition[0], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ypos", &obj->GetComponent<Transform>()->localPosition[1], 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zpos", &obj->GetComponent<Transform>()->localPosition[2], 0.001f);
    
    	Maths::Vec3 eulerRotation = obj->GetComponent<Transform>()->localRotation.EulerAngles();
		ImGui::Text(ICON_FA_ROTATE " Rotation: ");
		ImGui::SameLine(_textWidth - 16.f);
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
		obj->GetComponent<Transform>()->localRotation = Maths::Quaternion::EulerToQuaternion(
				Maths::Vec3(eulerRotation[0], eulerRotation[1], eulerRotation[2])); 

		ImGui::Text(ICON_FA_MAXIMIZE " Scale: ");
		// Scale is special case and needs some extra offset applied
		ImGui::SameLine(_textWidth);
		ImGui::InputFloat("##scale", &obj->GetComponent<Transform>()->localScale);
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
		scene.RemoveComponent(*obj->GetComponent<Camera>());
		return;
	}
	if (isHeaderOpen)
	{
		auto camera = obj->GetComponent<Camera>();

		{// the camera projection type
			if (Camera::ProjectionType::perspective == camera->projectionType)
			{
				std::strcpy(cameraType, _temp[0].c_str());
			}
			if (Camera::ProjectionType::orthographic == camera->projectionType)
			{

				std::strcpy(cameraType, _temp[1].c_str());
      }

			ImGui::Text("Projection Type");
			ImGui::SameLine(_textWidth);
			ImGui::SetNextItemWidth(_itemWidthMeshRenderComponent);
			ImGui::InputText("##Projection Type", cameraType, sizeof(cameraType), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::BeginPopupContextItem("projection type"))
			{
				if (ImGui::Selectable("Perspective"))
				{
					camera->projectionType = Camera::ProjectionType::perspective;
					//std::strcpy(cameraType, _temp[0].c_str());
				}
				if (ImGui::Selectable("Orthographic"))
				{
					camera->projectionType = Camera::ProjectionType::orthographic;
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
			if (Camera::ClearMode::backgroundColor == camera->clearMode)
			{
				std::strcpy(clearMode, _clearMode[2].c_str());
			}
			if (Camera::ClearMode::skybox == camera->clearMode)
			{
				std::strcpy(clearMode, _clearMode[1].c_str());
			}
			if (Camera::ClearMode::none == camera->clearMode)
			{
				std::strcpy(clearMode, _clearMode[0].c_str());
			}


			ImGui::Text("Clear Mode");
			ImGui::SameLine(_textWidth);
			ImGui::SetNextItemWidth(_itemWidthMeshRenderComponent);
			ImGui::InputText("##Clear Mode", clearMode, sizeof(clearMode), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::BeginPopupContextItem("clear mode"))
			{
				if (ImGui::Selectable("SkyBox"))
				{
					camera->clearMode = Camera::ClearMode::skybox;
				}
				if (ImGui::Selectable("None"))
				{
					camera->clearMode = Camera::ClearMode::none;
				}
				if (ImGui::Selectable("BackgroundColour"))
				{
					camera->clearMode = Camera::ClearMode::backgroundColor;
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
		ImGui::SameLine(_textWidth);
		ImGui::InputFloat("##FOV", &camera->fov);

		ImGui::Text("Near Clipping Plane: ");
		ImGui::SameLine(_textWidth);
		ImGui::InputFloat("##NearClippingPlane", &camera->nearClippingPlane);

		ImGui::Text("Far Clipping Plane: ");
		ImGui::SameLine(_textWidth);
		ImGui::InputFloat("##FarClippingPlane", &camera->farClippingPlane);

		ImGui::Text("Orthographic Size: ");
		ImGui::SameLine(_textWidth);
		ImGui::InputFloat("##OrthographicSize", &camera->orthographicSize);
	}
}

void InspectorWindow::ShowBoxColliderComponent(Scene& scene)
{
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_BOX "  BoxCollider Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	auto obj = _sceneEditor->GetSelectedGameobject();
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<BoxCollider>());
		return;
	}

	if(isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Center: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##XcenBox", &obj->GetComponent<BoxCollider>()->center.x, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##YcenBox", &obj->GetComponent<BoxCollider>()->center.y, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##ZcenBox", &obj->GetComponent<BoxCollider>()->center.z, 0.001f);

		ImGui::PushItemWidth(50);
		ImGui::Text("Size: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xsize", &obj->GetComponent<BoxCollider>()->size.x, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ysize", &obj->GetComponent<BoxCollider>()->size.y, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zsize", &obj->GetComponent<BoxCollider>()->size.z, 0.001f);
	}
}

void InspectorWindow::ShowCapsuleColliderComponent(Scene& scene)
{
	char directionBuff[64];

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAPSULES "  CapsuleCollider Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	auto obj = _sceneEditor->GetSelectedGameobject();
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<CapsuleCollider>());
		return;
	}

	if(isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Center: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##XcenCap", &obj->GetComponent<CapsuleCollider>()->center.x, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##YcenCap", &obj->GetComponent<CapsuleCollider>()->center.y, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##ZcenCap", &obj->GetComponent<CapsuleCollider>()->center.z, 0.001f);

		ImGui::PushItemWidth(50);
		ImGui::Text("Radius: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##RadiusCap", &obj->GetComponent<CapsuleCollider>()->radius, 0.001f);

		ImGui::PushItemWidth(50);
		ImGui::Text("Height: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Height", &obj->GetComponent<CapsuleCollider>()->height, 0.001f);
	}
}

void InspectorWindow::ShowSphereColliderComponent(Scene& scene)
{
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CIRCLE "  SphereCollider Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	auto obj = _sceneEditor->GetSelectedGameobject();
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<SphereCollider>());
		return;
	}

	if(isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Center: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xcen", &obj->GetComponent<SphereCollider>()->center.x, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ycen", &obj->GetComponent<SphereCollider>()->center.y, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zcen", &obj->GetComponent<SphereCollider>()->center.z, 0.001f);

		ImGui::PushItemWidth(50);
		ImGui::Text("Radius: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##RadiusSph", &obj->GetComponent<SphereCollider>()->radius, 0.001f);
	}
}

void InspectorWindow::ShowRigidBodyComponent(Scene& scene)
{
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_USER "  RigidBody Component", ImGuiTreeNodeFlags_AllowItemOverlap);
	auto obj = _sceneEditor->GetSelectedGameobject();
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRigidBodyComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<RigidBody>());
		return;
	}

	if(isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Mass: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Mass", &obj->GetComponent<RigidBody>()->mass, 0.001f);


		ImGui::PushItemWidth(50);
		ImGui::Text("IsKinematic: ");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##kinematic", &obj->GetComponent<RigidBody>()->kinematic);


		ImGui::PushItemWidth(50);
		ImGui::Text("Damping: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Damping", &obj->GetComponent<RigidBody>()->material.damping, 0.001f);


		ImGui::PushItemWidth(50);
		ImGui::Text("AngularDamping: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##AngularDamping", &obj->GetComponent<RigidBody>()->material.angularDamping, 0.001f);

		ImGui::Separator();
		ImGui::Text("Physical Material");
		ImGui::Separator();
		ImGui::PushItemWidth(50);
		ImGui::Text("Friction: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Friction", &obj->GetComponent<RigidBody>()->material.friction, 0.001f);

		ImGui::PushItemWidth(50);
		ImGui::Text("Bounciness: ");
		ImGui::SameLine(_textWidth);
		ImGui::SliderFloat("##Bounciness", &obj->GetComponent<RigidBody>()->material.bounciness, 0.f, 1.f, "%.2f");
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

	// TODO: Some of the sliders are wacky and need either resizing or changing to direct input

	if (isHeaderOpen)
	{
		auto component = obj->GetComponent<ParticleEffect>();

		if (ImGui::CollapsingHeader("Emitter Settings"))
		{
			ImGui::Text("Maximum Particles: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			ImGui::PushItemWidth(_itemWidthParticleEffectComponent);
			ImGui::SliderInt("##MaximumNumberOfParticles",&(component->particleEmitter->numberOfParticles),0.f, 5000, "%d", ImGuiSliderFlags_None);
			ImGui::PopItemWidth();

			ImGui::Text("Particle Lifetime: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			ImGui::PushItemWidth(_itemWidthParticleEffectComponent);

			ImGui::SliderFloat("##RespawnLifetime: ", &(component->particleEmitter->respawnLifetime), 0.f, 10, "%.3f", ImGuiSliderFlags_None);
			ImGui::PopItemWidth();

			ImGui::Text("New Particles: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			ImGui::PushItemWidth(_itemWidthParticleEffectComponent);
			ImGui::SliderInt("##NumberOfNewParticles",&(component->particleEmitter->numberOfNewParticles),0.f, 100, "%d", ImGuiSliderFlags_None);
			ImGui::PopItemWidth();

			ImGui::Text("Spawn Interval: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			ImGui::PushItemWidth(_itemWidthParticleEffectComponent);
			ImGui::SliderFloat("##SpawnIntervalParticles", &(component->particleEmitter->spawnInterval),0.016f, 5, "%.3f", ImGuiSliderFlags_None);
			ImGui::PopItemWidth();

			// TODO: Don't say acting force, Maybe have acceleration and calculate it properly in emitter?
			ImGui::Text("Acting Force: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall);

			ImGui::PushItemWidth(50);
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceX", &(component->particleEmitter->actingForce[0]));
			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceY", &(component->particleEmitter->actingForce[1]));
			ImGui::SameLine();

			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceZ", &(component->particleEmitter->actingForce[2]));
		}

		if (ImGui::CollapsingHeader("Position Settings"))
		{
			ImGui::Text("Initial Position: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall);

			ImGui::PushItemWidth(50);
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::InputFloat("##PositionX", &(component->particleEmitter->initPosition[0]));
			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::InputFloat("##PositionY", &(component->particleEmitter->initPosition[1]));
			ImGui::SameLine();

			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputFloat("##PositionZ", &(component->particleEmitter->initPosition[2]));

			ImGui::Text("Random X");
			ImGui::SameLine();
			ImGui::Checkbox("##RandomPositionX", &(component->particleEmitter->useRandomInitPositionX));
			ImGui::SameLine();

			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::InputFloat("##MinRandomPositionX", &(component->particleEmitter->minMaxPositionX[0]));
			ImGui::SameLine();

			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::InputFloat("##MaxRandomPositionX", &(component->particleEmitter->minMaxPositionX[1]));

			ImGui::Text("Random Y");
			ImGui::SameLine();
			ImGui::Checkbox("##RandomPositionY", &(component->particleEmitter->useRandomInitPositionY));
			ImGui::SameLine();

			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::InputFloat("##MinRandomPositionY", &(component->particleEmitter->minMaxPositionY[0]));
			ImGui::SameLine();

			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::InputFloat("##MaxRandomPositionY", &(component->particleEmitter->minMaxPositionY[1]));

			ImGui::Text("Random Z");
			ImGui::SameLine();
			ImGui::Checkbox("##RandomPositionZ", &(component->particleEmitter->useRandomInitPositionZ));
			ImGui::SameLine();

			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::InputFloat("##MinRandomPositionZ", &(component->particleEmitter->minMaxPositionZ[0]));
			ImGui::SameLine();

			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::InputFloat("##MaxRandomPositionZ", &(component->particleEmitter->minMaxPositionZ[1]));
			ImGui::PopItemWidth();
		}

		if (ImGui::CollapsingHeader("Velocity Settings"))
		{
			ImGui::Text("Initial Velocity: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall);

			ImGui::PushItemWidth(50);
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::InputFloat("##VelocityX", &(component->particleEmitter->initVelocity[0]));
			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::InputFloat("##VelocityY", &(component->particleEmitter->initVelocity[1]));
			ImGui::SameLine();

			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputFloat("##VelocityZ", &(component->particleEmitter->initVelocity[2]));

			ImGui::Text("Random X");
			ImGui::SameLine();
			ImGui::Checkbox("##RandomVelocityX", &(component->particleEmitter->useRandomInitVelocityX));
			ImGui::SameLine();

			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::InputFloat("##MinRandomVelocityX", &(component->particleEmitter->minMaxVelocityX[0]));
			ImGui::SameLine();

			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::InputFloat("##MaxRandomVelocityX", &(component->particleEmitter->minMaxVelocityX[1]));

			ImGui::Text("Random Y");
			ImGui::SameLine();
			ImGui::Checkbox("##RandomVelocityY", &(component->particleEmitter->useRandomInitVelocityY));
			ImGui::SameLine();

			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::InputFloat("##MinRandomVelocityY", &(component->particleEmitter->minMaxVelocityY[0]));
			ImGui::SameLine();

			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::InputFloat("##MaxRandomVelocityY", &(component->particleEmitter->minMaxVelocityY[1]));

			ImGui::Text("Random Z");
			ImGui::SameLine();
			ImGui::Checkbox("##RandomVelocityZ", &(component->particleEmitter->useRandomInitVelocityZ));
			ImGui::SameLine();

			ImGui::Text("Min");
			ImGui::SameLine();
			ImGui::InputFloat("##MinRandomVelocityZ", &(component->particleEmitter->minMaxVelocityZ[0]));
			ImGui::SameLine();

			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::InputFloat("##MaxRandomVelocityZ", &(component->particleEmitter->minMaxVelocityZ[1]));
			ImGui::PopItemWidth();
		}

		if (ImGui::CollapsingHeader("Shader Settings"))
		{
			// TODO: Scale shading stuff

			auto ColourPickerFlags =
					ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar;

			ImGui::Text("Start Colour: ");
			ImGui::SameLine();
			ImVec4 startColour((component->startColour[0]),
					(component->startColour[1]),
					(component->startColour[2]),
					(component->startColour[3]));
			ImGui::PushItemWidth(20);
			if (ImGui::ColorButton("##ButtonStartColour", startColour))
			{
				ImGui::OpenPopup("##PickStartColour");
			}
			ImGui::PopItemWidth();
			if (ImGui::BeginPopup("##PickStartColour"))
			{
				ImGui::PushItemWidth(180.0f);
				ImGui::ColorPicker4("##StartColour", (float*)&(component->startColour),
						ColourPickerFlags);
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			ImGui::SameLine();
			ImGui::Text("End Colour: ");
			ImGui::SameLine();
			ImVec4 endColour((component->endColour[0]),
					(component->endColour[1]),
					(component->endColour[2]),
					(component->endColour[3]));
			ImGui::PushItemWidth(20);
			if (ImGui::ColorButton("##ButtonEndColour", endColour))
			{
				ImGui::OpenPopup("##PickEndColour");
			}
			ImGui::PopItemWidth();
			if (ImGui::BeginPopup("##PickEndColour"))
			{
				ImGui::PushItemWidth(180.0f);
				ImGui::ColorPicker4("##EndColour", (float*)&(component->endColour), ColourPickerFlags);
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			// Possible options to shade by
			const std::string items[] = {"Life", "Position", "Size", "Speed"};
			ImGui::Text("Shade by: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall-30);
			ImGui::PushItemWidth(80);
			if (ImGui::BeginCombo("##ShadeByCombo", component->shadeBy.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (component->shadeBy == items[n]);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
						component->shadeBy = items[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			ImGui::Text("Min");
			float maxVal = 100.0f;
			if (component->shadeBy == "Life")
			{
				maxVal = component->particleEmitter->respawnLifetime;
				if (component->maxShadeValue > maxVal)
					component->maxShadeValue = maxVal;
				if (component->minShadeValue > maxVal)
					component->minShadeValue = maxVal;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::SliderFloat("##MinShadeValue", &component->minShadeValue, 0.f, maxVal, "%.3f", ImGuiSliderFlags_None);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::SliderFloat("##MaxShadeValue", &component->maxShadeValue, 0.f, maxVal, "%.3f", ImGuiSliderFlags_None);
			ImGui::PopItemWidth();

			char textureBuffer[64];
			if(component->particleEmitter->texture != nullptr)
				strcpy(textureBuffer,  component->particleEmitter->texture->fileName.c_str());
			else
				memset(textureBuffer, 0, 64 * sizeof(char));
			ImGui::Text("%s", "Texture:");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall-30);
			ImGui::PushItemWidth(_itemWidthParticleEffectComponent);
			ImGui::InputText("##ParticleEffectTexture", textureBuffer, sizeof(textureBuffer), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if(ImGui::Button("Select##ParticleEffectTexture"))
			{
				ImGui::OpenPopup("Select Particle Effect Texture");
			}
			{
				auto asset_Helper = _assetHelper->ShowGeneralTextureGuiWindow("Select Particle Effect Texture");
				if (std::get<0>(asset_Helper))
					component->particleEmitter->texture = std::get<1>(asset_Helper);
			}
			ImGui::SameLine();
			ImGui::Checkbox("##UseParticleEffectTexture", &(component->useTexture));

			ImGui::Text("Number of Rows: ");
			ImGui::SameLine();
			ImGui::InputInt("##NumberOfRowsInTexture", &(component->particleEmitter->numRowsInTexture));
			ImGui::Text("Number of Columns: ");
			ImGui::SameLine();
			ImGui::InputInt("##NumberOfColsInTexture", &(component->particleEmitter->numColsInTexture));
		}
	}
}

void InspectorWindow::ShowAudioComponent(Scene& scene)
{
	auto obj = _sceneEditor->GetSelectedGameobject();
	ImGui::Separator();
	char sampleBuffer[64];
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_TABLE_CELLS "  Audio", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRenderComponent")) {
		// remove component
		scene.RemoveComponent(*obj->GetComponent<AudioComponent>());
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Audio");
		ImGui::SameLine();
		ImGui::PushItemWidth(130.0f);

		// store the current mesh name into mesh buffer, so that we can display it in the input text box
		if(obj->GetComponent<AudioComponent>() != nullptr)
			strcpy(sampleBuffer,  obj->GetComponent<AudioComponent>()->fileName.c_str());
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
			obj->GetComponent<AudioComponent>()->Play();
		}

		auto asset_Helper = _assetHelper->ShowAudioGuiWindow();
		if(std::get<0>(asset_Helper))
		{
			obj->GetComponent<AudioComponent>()->LoadSample(std::get<1>(asset_Helper));
		}
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
				"RigidBody Component",
				"BoxCollider Component",
				"SphereCollider Component",
				"CapsuleCollider Component"
				"Particle Effect Component"
				"Audio Component"
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
				scene.AddComponent<MeshRender>(obj);
			}
			else if (strcmp(selectedComponent, "Transform Component") == 0)
			{
				// Add Transform Component
				scene.AddComponent<Transform>(obj);
			}
			else if (strcmp(selectedComponent, "Camera Component") == 0)
			{
				scene.AddComponent<Camera>(obj);
			}
			else if (strcmp(selectedComponent, "RigidBody Component") == 0)
			{
				if(obj && (obj->GetComponent<BoxCollider>() ||
				        				 obj->GetComponent<SphereCollider>() ||
				               			 obj->GetComponent<CapsuleCollider>()))
				{
					scene.AddComponent<RigidBody>(obj);
					_physics->AddRigidBody(obj);
				}
				else
					PLATINUM_WARNING("Please Add Collider First");
			}
			else if (strcmp(selectedComponent, "BoxCollider Component") == 0)
			{
				scene.AddComponent<BoxCollider>(obj);
			}
			else if (strcmp(selectedComponent, "SphereCollider Component") == 0)
			{
				scene.AddComponent<SphereCollider>(obj);
			}
			else if (strcmp(selectedComponent, "CapsuleCollider Component") == 0)
			{
				scene.AddComponent<CapsuleCollider>(obj);
			}
			else if (strcmp(selectedComponent, "Particle Effect Component") == 0)
			{
				// Add Particle Effect Component
				scene.AddComponent<ParticleEffect>(obj);
      }
			else if (strcmp(selectedComponent, "Audio Component") == 0)
			{
				scene.AddComponent<AudioComponent>(obj);
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
		ImGui::EndChild();
	}
}

std::filesystem::path InspectorWindow::GetPayloadPath(const ImGuiPayload* payload)
{
	char* payloadPointer = (char*)payload->Data;
	int size = payload->DataSize;
	std::string filePath = "";
	for(int i=0;i<size;i++)
		filePath+=*(payloadPointer+i);
	return std::filesystem::path(filePath);
}