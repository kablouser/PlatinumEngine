//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>
#include <ComponentComposition/Camera.h>

#include <SceneManager/Scene.h>
#include <Inspector/Bezier.h>

#include <Application.h>
#include <AssetDatabase/AssetHelper.h>
#include <SceneManager/SceneWithTemplates.h>


using namespace PlatinumEngine;

InspectorWindow::InspectorWindow() = default;

void InspectorWindow::ShowGUIWindow(bool* isOpen)
{
	if (ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector Window", isOpen))
	{
		auto& obj = Application::Instance->sceneEditor.GetSelectedGameobject();
		if (obj)
		{
			ImGui::Text("Name: ");
			ImGui::SameLine();
			static char objectNameBuffer[128];
			strcpy(objectNameBuffer, obj.DeRef()->name.c_str());
			ImGui::InputText("##input name", objectNameBuffer, IM_ARRAYSIZE(objectNameBuffer));
			obj.DeRef()->name = std::string(objectNameBuffer);

			ImGui::SameLine();
			bool isEnabled = obj.DeRef()->IsEnabled();

			if (ImGui::Checkbox("##IsEnabled", &isEnabled))
				obj.DeRef()->SetEnabled(isEnabled);

			// Now render each component gui
			if (auto ref = obj.DeRef()->GetComponent<MeshRender>())
				ShowMeshRenderComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<Transform>())
				ShowTransformComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<LightComponent>())
				ShowLightComponent(ref);
			
			if (auto ref = obj.DeRef()->GetComponent<Camera>())
				ShowCameraComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<RigidBody>())
				ShowRigidBodyComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<AnimationComponent>())
				ShowAnimationComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<AnimationAttachment>())
				ShowAnimationAttachmentComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<BoxCollider>())
				ShowBoxColliderComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<SphereCollider>())
				ShowSphereColliderComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<CapsuleCollider>())
				ShowCapsuleColliderComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<ParticleEffect>())
				ShowParticleEffectComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<AudioComponent>())
				ShowAudioComponent(ref);

			if (auto ref = obj.DeRef()->GetComponent<Player>())
				ShowPlayerComponent(ref);

			if (_isAddComponentWindowOpen)
				ShowAddComponent();
			else
			{
				if (ImGui::Button("Add Component"))
				{
					_isAddComponentWindowOpen = !_isAddComponentWindowOpen;
				}

				ImGui::SameLine();
				if (ImGui::Button("Remove Object"))
				{
					Application::Instance->sceneEditor.DeleteSelectedGameObject();
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

void InspectorWindow::ShowMeshRenderComponent(SavedReference<MeshRender>& reference)
{
	ImGui::Separator();
	std::string meshBuffer, textureBuffer, normalTextureBuffer;
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_VECTOR_SQUARE "  Mesh Render Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);

	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRenderComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
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
		if (reference.DeRef()->GetMesh())
			meshBuffer = reference.DeRef()->GetMesh().DeRef()->fileName;

		// show text box (read only)--------Choose Mesh
		ImGui::InputText("##Mesh Name", &meshBuffer[0], meshBuffer.size(), ImGuiInputTextFlags_ReadOnly);
		//Enables DragDrop for TextBox
		if (ImGui::BeginDragDropTarget())
		{
			//Accept any regular file (but it will check if it is mesh or not)
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
			{
				std::filesystem::path payloadPath = GetPayloadPath(payload);
				if (payloadPath.extension() == ".obj")
				{
					//Maybe we SetMesh on obj
					//obj->GetComponent<MeshRender>()->SetMesh(mesh);
					//Set The mesh that we dragged to the RenderComponent
					auto[success, meshAsset] = AssetHelper::GetAsset<Mesh>(payloadPath.string());
					if (success)
					{
						reference.DeRef()->SetMesh(meshAsset);

						if(SavedReference<AnimationComponent> animation = reference.DeRef()->GetComponent<AnimationComponent>(); animation)
						{

							animation.DeRef()->SetMesh(meshAsset);

						}
					}
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Select"))
		{
			ImGui::OpenPopup("Select Mesh");
		}

		{
			auto [isClicked, assetReference] = AssetHelper::PickAssetGUIWindow<Mesh>("Select Mesh");
			if (isClicked)
			{
				reference.DeRef()->SetMesh(assetReference);

				if(SavedReference<AnimationComponent> animation = reference.DeRef()->GetComponent<AnimationComponent>(); animation)
				{

					animation.DeRef()->SetMesh(assetReference);

				}
			}
		}

		if (reference.DeRef()->material.diffuseTexture)
			textureBuffer = reference.DeRef()->material.diffuseTexture.DeRef()->fileName;

		if (reference.DeRef()->material.normalTexture)
			normalTextureBuffer = reference.DeRef()->material.normalTexture.DeRef()->fileName;

		//Show text box (read only)----------Choose Material
		ImGui::Text("%s", "Material Properties");
		ImGui::Separator();

		ImGui::Text("Specular Exponent");
		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::SliderFloat("##shininess", &(reference.DeRef()->material.shininessFactor), 0.f, 100.f, "%.3f",
				ImGuiSliderFlags_None);
		ImGui::PopItemWidth();

		ImGui::Text("Blinn-Phong");
		ImGui::SameLine();
		ImGui::Checkbox("##Blinn-Phong", &(reference.DeRef()->material.useBlinnPhong));

		ImGui::Text("Texture");
		ImGui::SameLine(_textWidth);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);

		// store the current material name into mesh buffer, so that we can display it in the input text box

		ImGui::InputText("##Material Name", &textureBuffer[0], textureBuffer.size(), ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			//Accept any regular file (but it will check if it is texture or not)
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
			{
				std::filesystem::path payloadPath = GetPayloadPath(payload);
				if (payloadPath.extension() == ".png")
				{
					//Set The texture that we dragged to the RenderComponent
					auto [success, asset] = AssetHelper::GetAsset<Texture>(payloadPath.string());
					if (success)
					{
						reference.DeRef()->SetMaterial(asset);
						reference.DeRef()->material.useTexture = true;
					}
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("Select##Texture"))
		{
			ImGui::OpenPopup("Select Texture");
		}
		{
			auto [isClicked, assetReference] = AssetHelper::PickAssetGUIWindow<Texture>("Select Texture");
			if (isClicked)
			{
				reference.DeRef()->SetMaterial(assetReference);
				reference.DeRef()->material.useTexture = true;
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseTexture", &(reference.DeRef()->material.useTexture));

		ImGui::Text("%s", "Normal Map");
		ImGui::SameLine(_textWidth);
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::InputText("##Normal Map Name", &normalTextureBuffer[0], normalTextureBuffer.size(),
				ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			//Accept any regular file (but it will check if it is texture or not) [Same things as before]
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
			{
				std::filesystem::path payloadPath = GetPayloadPath(payload);
				if (payloadPath.extension() == ".png")
				{
					//Set The texture that we dragged to the RenderComponent
					auto [success, asset] = AssetHelper::GetAsset<Texture>(payloadPath.string());
					if (success)
						reference.DeRef()->SetNormalMap(asset);
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("Select##NormalTexture"))
		{
			ImGui::OpenPopup("Select Normal Texture");
		}
		{
			auto [isClicked, assetReference] = AssetHelper::PickAssetGUIWindow<Texture>("Select Normal Texture");
			if (isClicked)
				reference.DeRef()->SetNormalMap(assetReference);
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseNormalTexture", &(reference.DeRef()->material.useNormalTexture));

		ImGui::Text("Special Properties");
		ImGui::Separator();
		ImGui::Text("Reflection");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##UseRelfectionShader", &(reference.DeRef()->material.useReflectionShader));
		ImGui::Text("Refraction");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##UseRefracctionShader", &(reference.DeRef()->material.useRefractionShader));
		ImGui::Text("Refraction Index");
		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		// Setting max to 4 as you start to get unpleasing results past that
		ImGui::SliderFloat("##refractionIndex", &(reference.DeRef()->material.refractionIndex), 1.0f, 4.f,
				"%.3f", ImGuiSliderFlags_None);
		ImGui::PopItemWidth();

		auto ColourPickerFlags =
				ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar;
		ImGui::Text("Colour: ");
		ImGui::SameLine();
		ImVec4 startColour((reference.DeRef()->material.colour[0]),
				(reference.DeRef()->material.colour[1]),
				(reference.DeRef()->material.colour[2]), 1.0f);
		ImGui::PushItemWidth(20);
		if (ImGui::ColorButton("##ButtonDiffuseColour", startColour))
		{
			ImGui::OpenPopup("##PickDiffuseColour");
		}
		ImGui::PopItemWidth();
		if (ImGui::BeginPopup("##PickDiffuseColour"))
		{
			ImGui::PushItemWidth(180.0f);
			ImGui::ColorPicker4("##MaterialColour", (float*)&(reference.DeRef()->material.colour),
					ColourPickerFlags);
			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
	}
}

void InspectorWindow::ShowTransformComponent(SavedReference<Transform>& reference)
{
	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_ARROWS_TURN_TO_DOTS "  Transform Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveTransformComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}
	if (isHeaderOpen)
	{
		Transform* transformPointer = reference.DeRef().get();

		ImGui::PushItemWidth(50);
		ImGui::Text(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Position: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xpos", &transformPointer->localPosition[0], 0.01f, 0.0f, 0.0f, nullptr); // NO ROUNDING PLEASE
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ypos", &transformPointer->localPosition[1], 0.01f, 0.0f, 0.0f, nullptr); // NO ROUNDING PLEASE
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zpos", &transformPointer->localPosition[2], 0.01f, 0.0f, 0.0f, nullptr); // NO ROUNDING PLEASE

		float before = transformPointer->localScale;

		ImGui::Text(ICON_FA_ROTATE " Rotation: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xrot", &transformPointer->localRotation.x, 0.01f, 0.0f, 0.0f, nullptr); // NO ROUNDING PLEASE
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Yrot", &transformPointer->localRotation.y, 0.01f, 0.0f, 0.0f, nullptr); // NO ROUNDING PLEASE
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zrot", &transformPointer->localRotation.z, 0.01f, 0.0f, 0.0f, nullptr); // NO ROUNDING PLEASE
		ImGui::SameLine();
		ImGui::Text("W");
		ImGui::SameLine();
		ImGui::DragFloat("##Wrot", &transformPointer->localRotation.w, 0.01f, 0.0f, 0.0f, nullptr); // NO ROUNDING PLEASE
		transformPointer->localRotation = Maths::Quaternion::Normalise(transformPointer->localRotation);

		ImGui::Text(ICON_FA_MAXIMIZE " Scale: ");
		// Scale is special case and needs some extra offset applied
		ImGui::SameLine(_textWidthTransformComponent + 16.0f);
		ImGui::InputFloat("##scale", &transformPointer->localScale);
		ImGui::PopItemWidth();
	}

}

void InspectorWindow::ShowCameraComponent(SavedReference<Camera>& reference)
{
	char cameraType[64];
	char clearMode[64];

	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAMERA "  Camera Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveCameraComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}
	if (isHeaderOpen)
	{
		Camera* cameraPointer = reference.DeRef().get();

		{// the camera projection type
			if (Camera::ProjectionType::perspective == cameraPointer->projectionType)
			{
				std::strcpy(cameraType, _temp[0].c_str());
			}
			if (Camera::ProjectionType::orthographic == cameraPointer->projectionType)
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
					cameraPointer->projectionType = Camera::ProjectionType::perspective;
					//std::strcpy(cameraType, _temp[0].c_str());
				}
				if (ImGui::Selectable("Orthographic"))
				{
					cameraPointer->projectionType = Camera::ProjectionType::orthographic;
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
			if (Camera::ClearMode::backgroundColor == cameraPointer->clearMode)
			{
				std::strcpy(clearMode, _clearMode[2].c_str());
			}
			if (Camera::ClearMode::skybox == cameraPointer->clearMode)
			{
				std::strcpy(clearMode, _clearMode[1].c_str());
			}
			if (Camera::ClearMode::none == cameraPointer->clearMode)
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
					cameraPointer->clearMode = Camera::ClearMode::skybox;
				}
				if (ImGui::Selectable("None"))
				{
					cameraPointer->clearMode = Camera::ClearMode::none;
				}
				if (ImGui::Selectable("BackgroundColour"))
				{
					cameraPointer->clearMode = Camera::ClearMode::backgroundColor;
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
		ImGui::InputFloat("##Red", &cameraPointer->backgroundColor.r);
		ImGui::SameLine();

		ImGui::Text("G");
		ImGui::SameLine();
		ImGui::InputFloat("##Green", &cameraPointer->backgroundColor.g);
		ImGui::SameLine();

		ImGui::Text("B");
		ImGui::SameLine();
		ImGui::InputFloat("##Blue", &cameraPointer->backgroundColor.b);
		ImGui::SameLine();

		ImGui::Text("A");
		ImGui::SameLine();
		ImGui::InputFloat("##Alpha", &cameraPointer->backgroundColor.a);

		float fov;
		float nearClippingPlane;
		float farClippingPlane;
		float orthographicSize;

		ImGui::Text("Field of View: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##FOV", &cameraPointer->fov);

		ImGui::Text("Near Clipping Plane: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##NearClippingPlane", &cameraPointer->nearClippingPlane);

		ImGui::Text("Far Clipping Plane: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##FarClippingPlane", &cameraPointer->farClippingPlane);

		ImGui::Text("Orthographic Size: ");
		ImGui::SameLine(_textWidthCameraComponent);
		ImGui::InputFloat("##OrthographicSize", &cameraPointer->orthographicSize);
	}
}

void InspectorWindow::ShowBoxColliderComponent(SavedReference<BoxCollider>& reference)
{
	BoxCollider* boxColliderPointer = reference.DeRef().get();

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_BOX "  BoxCollider Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}

	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Size: ");
		ImGui::SameLine(_textWidth - 16.f);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##Xsize", &boxColliderPointer->size.x, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Ysize", &boxColliderPointer->size.y, 0.001f);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Zsize", &boxColliderPointer->size.z, 0.001f);
	}
}

void InspectorWindow::ShowCapsuleColliderComponent(SavedReference<CapsuleCollider>& reference)
{
	char directionBuff[64];

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAPSULES "  CapsuleCollider Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	CapsuleCollider* capsuleColliderPointer = reference.DeRef().get();

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}

	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Radius: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##RadiusCap", &capsuleColliderPointer->radius, 0.001f);

		ImGui::PushItemWidth(50);
		ImGui::Text("Height: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Height", &capsuleColliderPointer->height, 0.001f);
	}
}

void InspectorWindow::ShowSphereColliderComponent(SavedReference<SphereCollider>& reference)
{
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CIRCLE "  SphereCollider Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}

	if (isHeaderOpen)
	{
		SphereCollider* sphereCollider = reference.DeRef().get();

		ImGui::PushItemWidth(50);
		ImGui::Text("Radius: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##RadiusSph", &sphereCollider->radius, 0.001f);
	}
}

void InspectorWindow::ShowRigidBodyComponent(SavedReference<RigidBody>& reference)
{
	RigidBody* rigidBodyPointer = reference.DeRef().get();

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_USER "  RigidBody Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRigidBodyComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}

	if (isHeaderOpen)
	{
		ImGui::PushItemWidth(50);
		ImGui::Text("Mass: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Mass", &rigidBodyPointer->mass, 0.001f);


		ImGui::PushItemWidth(50);
		ImGui::Text("IsKinematic: ");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##kinematic", &rigidBodyPointer->isKinematic);


		ImGui::PushItemWidth(50);
		ImGui::Text("Damping: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Damping", &rigidBodyPointer->physicsMaterial.damping, 0.001f);


		ImGui::PushItemWidth(50);
		ImGui::Text("AngularDamping: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##AngularDamping", &rigidBodyPointer->physicsMaterial.angularDamping, 0.001f);

		ImGui::Separator();
		ImGui::Text("Physical Material");
		ImGui::Separator();
		ImGui::PushItemWidth(50);
		ImGui::Text("Friction: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##Friction", &rigidBodyPointer->physicsMaterial.friction, 0.001f);

		ImGui::PushItemWidth(50);
		ImGui::Text("Bounciness: ");
		ImGui::SameLine(_textWidth);
		ImGui::SliderFloat("##Bounciness", &rigidBodyPointer->physicsMaterial.bounciness, 0.f, 1.f, "%.2f");

		if (ImGui::Button("Update Physics Properties"))
			rigidBodyPointer->UpdatePhysicsProperties();
	}
}

void InspectorWindow::ShowParticleEffectComponent(SavedReference<ParticleEffect>& reference)
{
	// If this gui is being shown, assumption that object has component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_FIRE "  Particle Effect Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveParticleEffectComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}

	// TODO: Some of the sliders are wacky and need either resizing or changing to direct input

	if (isHeaderOpen)
	{
		ParticleEffect* particleEffectPointer = reference.DeRef().get();

		ImGui::Text("%s", "Playing");
		ImGui::SameLine();
		ImGui::Checkbox("##PlayingParticleEffect", &(particleEffectPointer->isPlaying));
		ImGui::SameLine();
		ImGui::Text("%s", "Emitting");
		ImGui::SameLine();
		ImGui::Checkbox("##BeginEmittingParticleEffect", &(particleEffectPointer->particleEmitter.isEmitting));
		ImGui::SameLine();
		if (ImGui::Button("OneShot##OneShotParticleEffect"))
			particleEffectPointer->particleEmitter.oneShot = true;

		ImGui::Text("%s", "Use Cylinder billboard");
		ImGui::SameLine();
		ImGui::Checkbox("##Use Cylinder billboard", &(particleEffectPointer->particleEmitter.useCylindricalBillboard));

		if (ImGui::CollapsingHeader("Emitter Settings"))
		{
			ImGui::PushItemWidth(_inputClampedWidth);

			ImGui::Text("Maximum Particles: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			InputIntClamped("##MaximumParticles", &particleEffectPointer->particleEmitter.numberOfParticles, 0, 5000);

			ImGui::Text("Particle Lifetime: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			InputFloatClamped("##ParticleLifetime", &particleEffectPointer->particleEmitter.respawnLifetime, 0.0f, 100.0f);

			ImGui::Text("New Particles: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			InputIntClamped("##NewParticles", &particleEffectPointer->particleEmitter.numberOfNewParticles, 0, 100);

			ImGui::Text("Spawn Interval: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			InputFloatClamped("##SpawnInterval", &particleEffectPointer->particleEmitter.spawnInterval, 0.016f, 100.0f);

			ImGui::Text("Acting Force: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);

			ImGui::PushItemWidth(50);
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceX", &(particleEffectPointer->particleEmitter.actingForce[0]));
			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceY", &(particleEffectPointer->particleEmitter.actingForce[1]));
			ImGui::SameLine();

			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceZ", &(particleEffectPointer->particleEmitter.actingForce[2]));
			ImGui::PopItemWidth();
			ImGui::PopItemWidth();
		}

		if (ImGui::CollapsingHeader("Particle Settings"))
		{
			// Scale
			{
				ImGui::Text("Scale: ");
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::InputFloat("##ScaleParticleX", &(particleEffectPointer->particleEmitter.scaleFactors.x));
				ImGui::SameLine();

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::InputFloat("##ScaleParticleY", &(particleEffectPointer->particleEmitter.scaleFactors.y));
				ImGui::SameLine();

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::InputFloat("##ScaleParticleZ", &(particleEffectPointer->particleEmitter.scaleFactors.z));

				ImGui::PopItemWidth();
			}

			ImGui::Separator();

			// Position
			{
				ImGui::Text("%s", "Random Initial Position");
				ImGui::PushItemWidth(50);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomPositionX", &(particleEffectPointer->particleEmitter.useRandomInitPositionX));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomPositionX", &(particleEffectPointer->particleEmitter.minPositionX));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomPositionX", &(particleEffectPointer->particleEmitter.maxPositionX));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformRandomPositionX", &(particleEffectPointer->particleEmitter.useUniformRandomPositionX));

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomPositionY", &(particleEffectPointer->particleEmitter.useRandomInitPositionY));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomPositionY", &(particleEffectPointer->particleEmitter.minPositionY));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomPositionY", &(particleEffectPointer->particleEmitter.maxPositionY));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformRandomPositionY", &(particleEffectPointer->particleEmitter.useUniformRandomPositionY));

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomPositionZ", &(particleEffectPointer->particleEmitter.useRandomInitPositionZ));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomPositionZ", &(particleEffectPointer->particleEmitter.minPositionZ));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomPositionZ", &(particleEffectPointer->particleEmitter.maxPositionZ));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformRandomPositionZ", &(particleEffectPointer->particleEmitter.useUniformRandomPositionZ));

				ImGui::PopItemWidth();
			}

			ImGui::Separator();

			// Velocity
			{
				ImGui::Text("%s", "Initial Velocity: ");
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::InputFloat("##VelocityX", &(particleEffectPointer->particleEmitter.initVelocity[0]));
				ImGui::SameLine();

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::InputFloat("##VelocityY", &(particleEffectPointer->particleEmitter.initVelocity[1]));
				ImGui::SameLine();

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::InputFloat("##VelocityZ", &(particleEffectPointer->particleEmitter.initVelocity[2]));

				ImGui::Text("%s", "Random Initial Velocity");

				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomVelocityX", &(particleEffectPointer->particleEmitter.useRandomInitVelocityX));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomVelocityX", &(particleEffectPointer->particleEmitter.minVelocityX));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomVelocityX", &(particleEffectPointer->particleEmitter.maxVelocityX));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformInitVelocityX", &(particleEffectPointer->particleEmitter.useUniformInitVelocityX));


				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomVelocityY", &(particleEffectPointer->particleEmitter.useRandomInitVelocityY));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomVelocityY", &(particleEffectPointer->particleEmitter.minVelocityY));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomVelocityY", &(particleEffectPointer->particleEmitter.maxVelocityY));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformInitVelocityY", &(particleEffectPointer->particleEmitter.useUniformInitVelocityY));

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomVelocityZ", &(particleEffectPointer->particleEmitter.useRandomInitVelocityZ));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomVelocityZ", &(particleEffectPointer->particleEmitter.minVelocityZ));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomVelocityZ", &(particleEffectPointer->particleEmitter.maxVelocityZ));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformInitVelocityZ", &(particleEffectPointer->particleEmitter.useUniformInitVelocityZ));

				ImGui::PopItemWidth();
			}
		}

		if (ImGui::CollapsingHeader("Shader Settings"))
		{
			auto ColourPickerFlags =
					ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar;

			ImGui::Text("Start Colour: ");
			ImGui::SameLine();
			ImVec4 startColour((particleEffectPointer->startColour[0]),
					(particleEffectPointer->startColour[1]),
					(particleEffectPointer->startColour[2]),
					(particleEffectPointer->startColour[3]));
			ImGui::PushItemWidth(20);
			if (ImGui::ColorButton("##ButtonStartColour", startColour))
			{
				ImGui::OpenPopup("##PickStartColour");
			}
			ImGui::PopItemWidth();
			if (ImGui::BeginPopup("##PickStartColour"))
			{
				ImGui::PushItemWidth(180.0f);
				ImGui::ColorPicker4("##StartColour", (float*)&(particleEffectPointer->startColour),
						ColourPickerFlags);
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			ImGui::SameLine();
			ImGui::Text("End Colour: ");
			ImGui::SameLine();
			ImVec4 endColour((particleEffectPointer->endColour[0]),
					(particleEffectPointer->endColour[1]),
					(particleEffectPointer->endColour[2]),
					(particleEffectPointer->endColour[3]));
			ImGui::PushItemWidth(20);
			if (ImGui::ColorButton("##ButtonEndColour", endColour))
			{
				ImGui::OpenPopup("##PickEndColour");
			}
			ImGui::PopItemWidth();
			if (ImGui::BeginPopup("##PickEndColour"))
			{
				ImGui::PushItemWidth(180.0f);
				ImGui::ColorPicker4("##EndColour", (float*)&(particleEffectPointer->endColour), ColourPickerFlags);
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			ImGui::Separator();

			// Possible options to shade by
			const std::string items[] = { "Life", "Position", "Size", "Speed" };
			ImGui::Text("Shade by: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall - 30);
			ImGui::PushItemWidth(90);
			if (ImGui::BeginCombo("##ShadeByCombo", particleEffectPointer->shadeBy.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (particleEffectPointer->shadeBy == items[n]);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
						particleEffectPointer->shadeBy = items[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::Text("Min");
			ImGui::SameLine();
			float maxVal = 100.0f; // default max value
			// If shading by life can only go to max life
			if (particleEffectPointer->shadeBy == "Life")
				maxVal = particleEffectPointer->particleEmitter.respawnLifetime;
			ImGui::PushItemWidth(_inputClampedWidth);
			InputFloatClamped("##MinShadeValue", &particleEffectPointer->minShadeValue, 0.f, maxVal);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::PushItemWidth(_inputClampedWidth);
			InputFloatClamped("##MaxShadeValue", &particleEffectPointer->maxShadeValue, 0.f, maxVal);
			ImGui::PopItemWidth();

			ImGui::Separator();

			const std::string scaleByItems[] = { "Constant", "Life", "Position", "Speed" };
			ImGui::Text("Scale by: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall - 30);
			ImGui::PushItemWidth(90);
			if (ImGui::BeginCombo("##ScaleByCombo", particleEffectPointer->particleEmitter.scaleBy.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(scaleByItems); n++)
				{
					bool is_selected = (particleEffectPointer->particleEmitter.scaleBy == scaleByItems[n]);
					if (ImGui::Selectable(scaleByItems[n].c_str(), is_selected))
						particleEffectPointer->particleEmitter.scaleBy = scaleByItems[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("%s", "Scale Factor: ");
			ImGui::SameLine();
			ImGui::PushItemWidth(_inputClampedWidth);
			InputFloatClamped("##ScaleFactorParticles", &particleEffectPointer->particleEmitter.scaleByFactor, 0.f, 100.0f);
			ImGui::PopItemWidth();

			ImGui::Separator();

			char textureBuffer[64];
			if (particleEffectPointer->particleEmitter.texture.DeRef() != nullptr)
				strcpy(textureBuffer, particleEffectPointer->particleEmitter.texture.DeRef()->fileName.c_str());
			else
				memset(textureBuffer, 0, 64 * sizeof(char));
			ImGui::Text("%s", "Texture: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall - 30);
			ImGui::PushItemWidth(_itemWidthParticleEffectComponent);
			ImGui::InputText("##ParticleEffectTexture", textureBuffer, sizeof(textureBuffer),
					ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Select##ParticleEffectTexture"))
			{
				ImGui::OpenPopup("Select Particle Effect Texture");
			}
			{
				auto [success, asset] = AssetHelper::PickAssetGUIWindow<Texture>("Select Particle Effect Texture");
				if (success)
					particleEffectPointer->particleEmitter.texture = asset;
			}
			ImGui::SameLine();
			ImGui::Checkbox("##UseParticleEffectTexture", &(particleEffectPointer->useTexture));

			ImGui::PushItemWidth(_inputClampedWidth);
			ImGui::Text("Number of Rows: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			InputIntClamped("##NumberOfRowsInTexture", &particleEffectPointer->particleEmitter.numRowsInTexture, 0, 99999);
			ImGui::Text("Number of Columns: ");
			ImGui::SameLine(_textWidthParticleEffectComponent);
			InputIntClamped("##NumberOfColsInTexture", &particleEffectPointer->particleEmitter.numColsInTexture, 0, 99999);
			ImGui::PopItemWidth();
		}
	}
}

void InspectorWindow::ShowAudioComponent(SavedReference<AudioComponent>& reference)
{
	AudioComponent* audioComponentPointer = reference.DeRef().get();

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_MUSIC "  Audio", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveAudioComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Audio");
		ImGui::SameLine(_textWidthAudioComponent);
		ImGui::PushItemWidth(_itemWidthAudioComponent);

		// store the current audio name into audio buffer, so that we can display it in the input text box
		std::string emptyString;
		std::string* audioClipName = nullptr;
		if(audioComponentPointer->audioClip)
			audioClipName = &audioComponentPointer->audioClip.DeRef()->fileName;
		else
			audioClipName = &emptyString;

		// show text box (read only)
		ImGui::InputText("##Sample Name",audioClipName->data(),audioClipName->size(), ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget())
		{
			//Accept any regular file (but it will check if it is texture or not)
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RegularFilePathPayload"))
			{
				std::filesystem::path payloadPath = GetPayloadPath(payload);

				if(payloadPath.extension()==".wav")
				{
					//Set the sample that we dragged to the AudioComponent
					auto asset_Helper = AssetHelper::GetAsset<AudioClip>(payloadPath.string());
					if (std::get<0>(asset_Helper))
						audioComponentPointer->audioClip = (std::get<1>(asset_Helper));
				}
			}
			// End DragDropTarget
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		const char SELECT_AUDIO_CLIP[] = "Select Audio Clip";

		if (ImGui::Button("Choose sample"))
		{
			ImGui::OpenPopup(SELECT_AUDIO_CLIP);
		}

		if (ImGui::Button("Play"))
		{
			audioComponentPointer->Play();
		}
		ImGui::SameLine();
		if(ImGui::Button("Pause"))
		{
			audioComponentPointer->Pause();
		}
		ImGui::SameLine();
		if(ImGui::Button("Stop"))
		{
			audioComponentPointer->Stop();
		}

		if(ImGui::Checkbox("Looping",&audioComponentPointer->isLooping))
		{
			if(audioComponentPointer->IsPlaying())
				audioComponentPointer->Stop();
		}

		int volume = audioComponentPointer->GetVolume();
		ImGui::SliderInt("Volume", &volume, 0, 128);
		audioComponentPointer->SetVolume(volume);

		int panning = audioComponentPointer->GetPanning();
		std::string panningLabel = (panning==127)?"C":((panning>127)?"R: "+std::to_string(panning-127):"L: "+std::to_string(127-panning));
		ImGui::SliderInt("Panning", &panning, 0, 254, panningLabel.c_str());
		audioComponentPointer->SetPanning(panning);

		int channel = audioComponentPointer->GetChannel();
		ImGui::Text("CHANNEL: %d",channel);

		auto [success, asset] = AssetHelper::PickAssetGUIWindow<AudioClip>(SELECT_AUDIO_CLIP);
		if (success)
		{
			audioComponentPointer->audioClip = asset;
		}
	}
}

void InspectorWindow::ShowAnimationComponent(SavedReference<AnimationComponent>& reference)
{
	ImGui::Separator();
	char meshBuffer[64];
	char textureBuffer[64];
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_PERSON_FALLING "  Animation", ImGuiTreeNodeFlags_AllowItemOverlap);

	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveAnimationComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}
	if (isHeaderOpen)
	{

		// store pointer of renderComponent
		AnimationComponent* animationComponentPointer = reference.DeRef().get();

		bool isDisplayed = animationComponentPointer->GetIsDisplay();
		if (ImGui::Checkbox("Display Animation", &isDisplayed))
		{
			animationComponentPointer->SetIsDisplay(isDisplayed);
		}

		for (unsigned int i = 0; i < animationComponentPointer->GetAmountOfAnimations(); ++i)
		{
			// create check box
			if (ImGui::RadioButton(
					std::to_string(i).append(". ").append(animationComponentPointer->GetAnimationName(i)).c_str(),
					animationComponentPointer->GetCurrentAnimationID() == i))
			{
				// set the animation presented by this check box to be the current selected animation
				animationComponentPointer->SetCurrentAnimationByID(i);
			}
		}

		ImGui::InputFloat("Time##animationTime", &animationComponentPointer->timer.animationTime);
		ImGui::Checkbox("Is Looping##isAnimationLooping", &animationComponentPointer->timer.isAnimationLooping);
		ImGui::Checkbox("Is Playing##isAnimationPlaying", &animationComponentPointer->timer.isAnimationPlaying);
	}
}


void InspectorWindow::ShowPlayerComponent(SavedReference<Player>& reference)
{
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_AllowItemOverlap);
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);

	if (ImGui::Button("X##RemovePlayerAttachmentComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}

	if (isHeaderOpen)
	{
		// store pointer of renderComponent
		Player* player = reference.DeRef().get();

		ImGui::InputFloat("Move Speed##Player", &player->moveSpeed);
		ImGui::InputFloat("Move Acceleration##Player", &player->moveAcceleration);
		ImGui::InputFloat("Move Deceleration##Player", &player->moveDeceleration);
		ImGui::InputFloat("Jump Speed##Player", &player->jumpSpeed);
	}
}

void InspectorWindow::ShowAnimationAttachmentComponent(SavedReference<AnimationAttachment>& reference)
{
	ImGui::Separator();
	char meshBuffer[64];
	char textureBuffer[64];
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_WAND_MAGIC "  Animation Attachment", ImGuiTreeNodeFlags_AllowItemOverlap);

	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveAnimationAttachmentComponent"))
	{
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}
	if (isHeaderOpen)
	{


		// store pointer of renderComponent
		AnimationAttachment* animationAttachment = reference.DeRef().get();

		if(animationAttachment == nullptr)
			return;

		// update joints' names
		animationAttachment->GetSkeletonNameFromParentAnimation();
		std::vector<std::string>& jointNames = animationAttachment->jointsName;

		if(jointNames.empty())
			return;


		// select box
		if(ImGui::BeginCombo("Joints", jointNames[animationAttachment->GetSelectedJoint()].c_str(), 0))
		{
			for(unsigned int i =0; i<jointNames.size(); i++)
			{
				bool is_selected = (i == animationAttachment->GetSelectedJoint());

				if (ImGui::Selectable(jointNames[i].c_str(), is_selected))
				{
					animationAttachment->SetSelectedJoint(i);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo(); // only call EndCombo() if BeginCombo() returns true!
		}

		// input boxes
		float x=animationAttachment->translation.x,
		y= animationAttachment->translation.y,
		z= animationAttachment->translation.z;
		ImGui::Text(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Position: ");;
		ImGui::SameLine();
		ImGui::Text("x");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::InputFloat("##attachTranslationx", &x);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text("y");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::InputFloat("##attachTranslationy", &y);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text("z");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::InputFloat("##attachTranslationz", &z);
		ImGui::PopItemWidth();

		float rx=animationAttachment->rotation.x,
		ry=animationAttachment->rotation.y,
		rz=animationAttachment->rotation.z;
		ImGui::Text(ICON_FA_ROTATE " Rotation: ");;
		ImGui::SameLine();
		ImGui::Text("x");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::InputFloat("##attachRotationx", &rx);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text("y");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::InputFloat("##attachRotationy", &ry);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text("z");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::InputFloat("##attachRotationz", &rz);
		ImGui::PopItemWidth();

		ImGui::Text(ICON_FA_MAXIMIZE " Scale: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::InputFloat("##attachScale", &animationAttachment->scale);
		ImGui::PopItemWidth();

		animationAttachment->UpdateOffsetMatrix(Maths::Vec3(x, y, z), Maths::Vec3(rx,ry,rz), animationAttachment->scale);

	}
}

void InspectorWindow::ShowAddComponent()
{
	if (ImGui::BeginChild("ComponentSelector"))
	{
		ImGui::Separator();

		auto& obj = Application::Instance->sceneEditor.GetSelectedGameobject();

		static const char* components[] = {
				"Mesh Render Component",
				"Transform Component",
				"Camera Component",
				"Light Component",
				"RigidBody Component",
				"BoxCollider Component",
				"SphereCollider Component",
				"CapsuleCollider Component",
				"Particle Effect Component",
				"Audio Component",
				"Animation Component",
				"Player",
				"Animation Attachment Component"
		};
		static const char* selectedComponent = nullptr;
		static ImGuiTextFilter filter;
		ImGui::Text("%s", "Search Components");
		ImGui::SameLine();
		filter.Draw( ICON_FA_MAGNIFYING_GLASS);
		for (const auto component : components)
		{
			// Only add components that match search string
			if (filter.PassFilter(component))
			{
				// If found, display component
				if (ImGui::Selectable(component))
				{
					selectedComponent = component;
				}
			}
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
				Application::Instance->scene.AddComponent<MeshRender>(obj);
			}
			else if (strcmp(selectedComponent, "Transform Component") == 0)
			{
				// Add Transform Component
				Application::Instance->scene.AddComponent<Transform>(obj);
			}
			else if (strcmp(selectedComponent, "Camera Component") == 0)
			{
				Application::Instance->scene.AddComponent<Camera>(obj);
			}
			else if (strcmp(selectedComponent, "RigidBody Component") == 0)
			{
				Application::Instance->scene.AddComponent<RigidBody>(obj);
			}
			else if (strcmp(selectedComponent, "BoxCollider Component") == 0)
			{
				Application::Instance->scene.AddComponent<BoxCollider>(obj);
			}
			else if (strcmp(selectedComponent, "SphereCollider Component") == 0)
			{
				Application::Instance->scene.AddComponent<SphereCollider>(obj);
			}
			else if (strcmp(selectedComponent, "CapsuleCollider Component") == 0)
			{
				Application::Instance->scene.AddComponent<CapsuleCollider>(obj);
			}
			else if (strcmp(selectedComponent, "Particle Effect Component") == 0)
			{
				// Add Particle Effect Component
				Application::Instance->scene.AddComponent<ParticleEffect>(obj);
			}
			else if (strcmp(selectedComponent, "Audio Component") == 0)
			{
				Application::Instance->scene.AddComponent<AudioComponent>(obj);
			}
			else if (strcmp(selectedComponent, "Animation Component") == 0)
			{
				Application::Instance->scene.AddComponent<AnimationComponent>(obj);

				if(obj.DeRef()->GetComponent<MeshRender>())
				{
					obj.DeRef()->GetComponent<AnimationComponent>().DeRef()->SetMesh(obj.DeRef()->GetComponent<MeshRender>().DeRef()->GetMesh());
				}
			}
			else if (strcmp(selectedComponent, "Light Component") == 0)
			{
				Application::Instance->scene.AddComponent<LightComponent>(obj);
			}
			else if (strcmp(selectedComponent, "Player") == 0)
			{
				Application::Instance->scene.AddComponent<Player>(obj);
			}

			else if (strcmp(selectedComponent, "Animation Attachment Component") == 0)
			{
				Application::Instance->scene.AddComponent<AnimationAttachment>(obj);
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
	}
	ImGui::EndChild();
}

void InspectorWindow::ShowLightComponent(SavedReference<LightComponent>& reference)
{
	// If this gui is being shown, assumption that object has light component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_LIGHTBULB "   Light Component", ImGuiTreeNodeFlags_AllowItemOverlap);

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveLightComponent")) {
		// remove component
		Application::Instance->scene.RemoveComponent(reference);
		return;
	}
	if (isHeaderOpen)
	{
		auto& light = reference;
		if(ImGui::Combo("Type", (int*)&light.DeRef()->type, light.DeRef()->LightTypeNames, (int)LightComponent::LightType::count))
		{
			light.DeRef()->UpdateMesh();
		}

		ImGui::ColorEdit3("Spectrum", light.DeRef()->spectrum.data);
		ImGui::DragFloat("Intensity", &light.DeRef()->intensity, 0.1f, 0.0f,
				std::numeric_limits<float>::max(), "%.2f");
		if (light.DeRef()->type == LightComponent::LightType::Point)
		{
			ImGui::DragFloat("Constant", &light.DeRef()->constant);
			ImGui::DragFloat("Linear", &light.DeRef()->linear);
			ImGui::DragFloat("Quadratic", &light.DeRef()->quadratic, 1.0f, 0.0f, 0.0f, "%.6f");
		}
	}
}

std::filesystem::path InspectorWindow::GetPayloadPath(const ImGuiPayload* payload)
{
	char* payloadPointer = (char*)payload->Data;
	int size = payload->DataSize;
	std::string filePath = "";
	for (int i = 0; i < size; i++)
		filePath += *(payloadPointer + i);
	return std::filesystem::path(filePath);
}

void InspectorWindow::InputIntClamped(const char *label, int *val, int min, int max)
{
	int inputVal = *val;
	ImGui::InputInt(label, &inputVal, 1, 10);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		inputVal = std::clamp(inputVal, min, max);
		*val = inputVal;
	}
}

void InspectorWindow::InputFloatClamped(const char *label, float *val, float min, float max) {
	float inputVal = *val;
	ImGui::InputFloat(label, &inputVal, 0.1f, 1.0f);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		inputVal = std::clamp(inputVal, min, max);
		*val = inputVal;
	}
}