//
// Created by Matt & Shawn on 27/03/2022.
//

#include <Inspector/InspectorWindow.h>
#include <ComponentComposition/Camera.h>

#include <ImGuizmo.h>
#include <SceneManager/Scene.h>
#include <Inspector/Bezier.h>


using namespace PlatinumEngine;

InspectorWindow::InspectorWindow(AssetHelper* assetHelper, SceneEditor* sceneEditor, Physics* physics) :
		_assetHelper(assetHelper), _sceneEditor(sceneEditor), _physics(physics)
{
}

void InspectorWindow::ShowGUIWindow(bool* isOpen, Scene& scene)
{
	if (ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector Window", isOpen))
	{
		auto& obj = _sceneEditor->GetSelectedGameobject();
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
			{
				obj.DeRef()->SetEnabled(isEnabled, scene);
			}

			// Now render each component gui
			if (obj.DeRef()->GetComponent<MeshRender>())
				ShowMeshRenderComponent(scene);

			if (obj.DeRef()->GetComponent<Transform>())
				ShowTransformComponent(scene);

			if (obj.DeRef()->GetComponent<Camera>())
				ShowCameraComponent(scene);

			if (obj.DeRef()->GetComponent<RigidBody>())
				ShowRigidBodyComponent(scene);

			if (obj.DeRef()->GetComponent<AnimationComponent>())
				ShowAnimationComponent(scene);

			if (obj.DeRef()->GetComponent<BoxCollider>())
				ShowBoxColliderComponent(scene);

			if (obj.DeRef()->GetComponent<SphereCollider>())
				ShowSphereColliderComponent(scene);

			if (obj.DeRef()->GetComponent<CapsuleCollider>())
				ShowCapsuleColliderComponent(scene);

			if (obj.DeRef()->GetComponent<ParticleEffect>())
				ShowParticleEffectComponent(scene);

			if (obj.DeRef()->GetComponent<AudioComponent>())
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
	std::string meshBuffer, textureBuffer, normalTextureBuffer;
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_VECTOR_SQUARE "  Mesh Render Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	SavedReference<GameObject>& obj = _sceneEditor->GetSelectedGameobject();
	SavedReference<MeshRender> meshRender = obj.DeRef()->GetComponent<MeshRender>();

	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRenderComponent"))
	{
		// remove component
		scene.RemoveComponent(meshRender);
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
		if (meshRender.DeRef()->GetMesh())
			meshBuffer = meshRender.DeRef()->GetMesh().DeRef()->fileName;

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
					auto[success, meshAsset] = _assetHelper->GetAsset<Mesh>(payloadPath.string());
					if (success)
					{
						meshRender.DeRef()->SetMesh(meshAsset);

						if(SavedReference<AnimationComponent> animation = obj.DeRef()->GetComponent<AnimationComponent>(); animation)
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
			auto [isClicked, assetReference] = _assetHelper->PickAssetGUIWindow<Mesh>("Select Mesh");
			if (isClicked)
			{
				meshRender.DeRef()->SetMesh(assetReference);

				if(SavedReference<AnimationComponent> animation = obj.DeRef()->GetComponent<AnimationComponent>(); animation)
				{

					animation.DeRef()->SetMesh(assetReference);

				}
			}
		}

		if (meshRender.DeRef()->material.diffuseTexture)
			textureBuffer = meshRender.DeRef()->material.diffuseTexture.DeRef()->fileName;

		if (meshRender.DeRef()->material.normalTexture)
			normalTextureBuffer = meshRender.DeRef()->material.normalTexture.DeRef()->fileName;

		//Show text box (read only)----------Choose Material
		ImGui::Text("%s", "Material Properties");
		ImGui::Separator();

		ImGui::Text("Specular Exponent");
		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		ImGui::SliderFloat("##shininess", &(meshRender.DeRef()->material.shininessFactor), 0.f, 100.f, "%.3f",
				ImGuiSliderFlags_None);
		ImGui::PopItemWidth();

		ImGui::Text("Blinn-Phong");
		ImGui::SameLine();
		ImGui::Checkbox("##Blinn-Phong", &(meshRender.DeRef()->material.useBlinnPhong));

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
					auto [success, asset] = _assetHelper->GetAsset<Texture>(payloadPath.string());
					if (success)
					{
						meshRender.DeRef()->SetMaterial(asset);
						meshRender.DeRef()->material.useTexture = true;
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
			auto [isClicked, assetReference] = _assetHelper->PickAssetGUIWindow<Texture>("Select Texture");
			if (isClicked)
			{
				meshRender.DeRef()->SetMaterial(assetReference);
				meshRender.DeRef()->material.useTexture = true;
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseTexture", &(meshRender.DeRef()->material.useTexture));

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
					auto [success, asset] = _assetHelper->GetAsset<Texture>(payloadPath.string());
					if (success)
						meshRender.DeRef()->SetNormalMap(asset);
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
			auto [isClicked, assetReference] = _assetHelper->PickAssetGUIWindow<Texture>("Select Normal Texture");
			if (isClicked)
				meshRender.DeRef()->SetNormalMap(assetReference);
		}
		ImGui::SameLine();
		ImGui::Checkbox("##UseNormalTexture", &(meshRender.DeRef()->material.useNormalTexture));

		ImGui::Text("Special Properties");
		ImGui::Separator();
		ImGui::Text("Reflection");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##UseRelfectionShader", &(meshRender.DeRef()->material.useReflectionShader));
		ImGui::Text("Refraction");
		ImGui::SameLine(_textWidth);
		ImGui::Checkbox("##UseRefracctionShader", &(meshRender.DeRef()->material.useRefractionShader));
		ImGui::Text("Refraction Index");
		ImGui::SameLine();
		ImGui::PushItemWidth(_itemWidthMeshRenderComponent);
		// Setting max to 4 as you start to get unpleasing results past that
		ImGui::SliderFloat("##refractionIndex", &(meshRender.DeRef()->material.refractionIndex), 1.0f, 4.f,
				"%.3f", ImGuiSliderFlags_None);
		ImGui::PopItemWidth();
	}
}

void InspectorWindow::ShowTransformComponent(Scene& scene)
{
	auto& obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_ARROWS_TURN_TO_DOTS "  Transform Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveTransformComponent"))
	{
		// remove component
		scene.RemoveComponent(obj.DeRef()->GetComponent<Transform>());
		return;
	}
	if (isHeaderOpen)
	{
		Transform* transformPointer = obj.DeRef()->GetComponent<Transform>().DeRef().get();

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

void InspectorWindow::ShowCameraComponent(Scene& scene)
{
	char cameraType[64];
	char clearMode[64];
	auto& obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has transform component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAMERA "  Camera Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveCameraComponent"))
	{
		// remove component
		scene.RemoveComponent(obj.DeRef()->GetComponent<Camera>());
		return;
	}
	if (isHeaderOpen)
	{
		auto camera = obj.DeRef()->GetComponent<Camera>();

		{// the camera projection type
			if (Camera::ProjectionType::perspective == camera.DeRef()->projectionType)
			{
				std::strcpy(cameraType, _temp[0].c_str());
			}
			if (Camera::ProjectionType::orthographic == camera.DeRef()->projectionType)
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
					camera.DeRef()->projectionType = Camera::ProjectionType::perspective;
					//std::strcpy(cameraType, _temp[0].c_str());
				}
				if (ImGui::Selectable("Orthographic"))
				{
					camera.DeRef()->projectionType = Camera::ProjectionType::orthographic;
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
			if (Camera::ClearMode::backgroundColor == camera.DeRef()->clearMode)
			{
				std::strcpy(clearMode, _clearMode[2].c_str());
			}
			if (Camera::ClearMode::skybox == camera.DeRef()->clearMode)
			{
				std::strcpy(clearMode, _clearMode[1].c_str());
			}
			if (Camera::ClearMode::none == camera.DeRef()->clearMode)
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
					camera.DeRef()->clearMode = Camera::ClearMode::skybox;
				}
				if (ImGui::Selectable("None"))
				{
					camera.DeRef()->clearMode = Camera::ClearMode::none;
				}
				if (ImGui::Selectable("BackgroundColour"))
				{
					camera.DeRef()->clearMode = Camera::ClearMode::backgroundColor;
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

void InspectorWindow::ShowBoxColliderComponent(Scene& scene)
{
	SavedReference<BoxCollider> boxCollider = _sceneEditor->GetSelectedGameobject().DeRef()->GetComponent<BoxCollider>();
	BoxCollider* boxColliderPointer = boxCollider.DeRef().get();

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_BOX "  BoxCollider Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent"))
	{
		// remove component
		scene.RemoveComponent(boxCollider);
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

void InspectorWindow::ShowCapsuleColliderComponent(Scene& scene)
{
	char directionBuff[64];

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CAPSULES "  CapsuleCollider Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	SavedReference<CapsuleCollider> capsuleCollider = _sceneEditor->GetSelectedGameobject().DeRef()->GetComponent<CapsuleCollider>();
	CapsuleCollider* capsuleColliderPointer = capsuleCollider.DeRef().get();

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent"))
	{
		// remove component
		scene.RemoveComponent(capsuleCollider);
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

void InspectorWindow::ShowSphereColliderComponent(Scene& scene)
{
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_CIRCLE "  SphereCollider Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	auto obj = _sceneEditor->GetSelectedGameobject();
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveBoxColliderComponent"))
	{
		// remove component
		scene.RemoveComponent(obj.DeRef()->GetComponent<SphereCollider>());
		return;
	}

	if (isHeaderOpen)
	{
		SphereCollider* sphereCollider = obj.DeRef()->GetComponent<SphereCollider>().DeRef().get();

		ImGui::PushItemWidth(50);
		ImGui::Text("Radius: ");
		ImGui::SameLine(_textWidth);
		ImGui::DragFloat("##RadiusSph", &sphereCollider->radius, 0.001f);
	}
}

void InspectorWindow::ShowRigidBodyComponent(Scene& scene)
{
	if (!_sceneEditor->GetSelectedGameobject())
		return;
	SavedReference<RigidBody> rigidBody = _sceneEditor->GetSelectedGameobject().DeRef()->GetComponent<RigidBody>();
	if (!rigidBody)
		return;
	RigidBody* rigidBodyPointer = rigidBody.DeRef().get();

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_USER " RigidBody Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);

	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveRigidBodyComponent"))
	{
		// remove component
		scene.RemoveComponent(rigidBody);
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
			rigidBodyPointer->UpdatePhysicsProperties(*_physics);
	}
}

void InspectorWindow::ShowParticleEffectComponent(Scene& scene)
{
	auto obj = _sceneEditor->GetSelectedGameobject();

	// If this gui is being shown, assumption that object has component
	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_FIRE "  Particle Effect Component",
			ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveParticleEffectComponent"))
	{
		// remove component
		scene.RemoveComponent(obj.DeRef()->GetComponent<ParticleEffect>());
		return;
	}

	// TODO: Some of the sliders are wacky and need either resizing or changing to direct input

	if (isHeaderOpen)
	{
		auto component = obj.DeRef()->GetComponent<ParticleEffect>();

		ImGui::Text("%s", "Emitting");
		ImGui::SameLine();
		ImGui::Checkbox("##BeginEmittingParticleEffect", &(component.DeRef()->particleEmitter.isEmitting));
		ImGui::SameLine();
		if (ImGui::Button("OneShot##OneShotParticleEffect"))
			component.DeRef()->particleEmitter.oneShot = true;
		ImGui::SameLine();
		ImGui::Text("%s", "Playing");
		ImGui::SameLine();
		ImGui::Checkbox("##PlayingParticleEffect", &(component.DeRef()->isPlaying));

		if (ImGui::CollapsingHeader("Emitter Settings"))
		{
			ImGui::Text("Maximum Particles: ");
			ImGui::SliderInt("##MaximumNumberOfParticles", &(component.DeRef()->particleEmitter.numberOfParticles), 0.f, 5000,
					"%d", ImGuiSliderFlags_None);

			ImGui::Text("Particle Lifetime: ");
			ImGui::SliderFloat("##RespawnLifetime: ", &(component.DeRef()->particleEmitter.respawnLifetime), 0.f, 10, "%.3f",
					ImGuiSliderFlags_None);

			ImGui::Text("New Particles: ");
			ImGui::SliderInt("##NumberOfNewParticles", &(component.DeRef()->particleEmitter.numberOfNewParticles), 0.f, 100,
					"%d", ImGuiSliderFlags_None);

			ImGui::Text("Spawn Interval: ");
			ImGui::SliderFloat("##SpawnIntervalParticles", &(component.DeRef()->particleEmitter.spawnInterval), 0.016f, 5,
					"%.3f", ImGuiSliderFlags_None);

			ImGui::Text("Acting Force: ");

			ImGui::PushItemWidth(50);
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceX", &(component.DeRef()->particleEmitter.actingForce[0]));
			ImGui::SameLine();

			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceY", &(component.DeRef()->particleEmitter.actingForce[1]));
			ImGui::SameLine();

			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputFloat("##ActingForceZ", &(component.DeRef()->particleEmitter.actingForce[2]));
		}

		if (ImGui::CollapsingHeader("Particle Settings"))
		{
			// Scale
			{
				ImGui::Text("Scale");
				ImGui::PushItemWidth(50);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::InputFloat("##ScaleParticleX", &(component.DeRef()->particleEmitter.scaleFactors.x));
				ImGui::SameLine();

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::InputFloat("##ScaleParticleY", &(component.DeRef()->particleEmitter.scaleFactors.y));
				ImGui::SameLine();

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::InputFloat("##ScaleParticleZ", &(component.DeRef()->particleEmitter.scaleFactors.z));
			}

			ImGui::Separator();

			// Position
			{
				ImGui::Text("%s", "Random Position");
				ImGui::PushItemWidth(50);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomPositionX", &(component.DeRef()->particleEmitter.useRandomInitPositionX));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomPositionX", &(component.DeRef()->particleEmitter.minPositionX));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomPositionX", &(component.DeRef()->particleEmitter.maxPositionX));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformRandomPositionX", &(component.DeRef()->particleEmitter.useUniformRandomPositionX));

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomPositionY", &(component.DeRef()->particleEmitter.useRandomInitPositionY));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomPositionY", &(component.DeRef()->particleEmitter.minPositionY));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomPositionY", &(component.DeRef()->particleEmitter.maxPositionY));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformRandomPositionY", &(component.DeRef()->particleEmitter.useUniformRandomPositionY));

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomPositionZ", &(component.DeRef()->particleEmitter.useRandomInitPositionZ));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomPositionZ", &(component.DeRef()->particleEmitter.minPositionZ));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomPositionZ", &(component.DeRef()->particleEmitter.maxPositionZ));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformRandomPositionZ", &(component.DeRef()->particleEmitter.useUniformRandomPositionZ));

				ImGui::PopItemWidth();
			}

			ImGui::Separator();

			// Velocity
			{
				ImGui::Text("%s", "Default Initial Velocity");
				ImGui::PushItemWidth(50);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::InputFloat("##VelocityX", &(component.DeRef()->particleEmitter.initVelocity[0]));
				ImGui::SameLine();

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::InputFloat("##VelocityY", &(component.DeRef()->particleEmitter.initVelocity[1]));
				ImGui::SameLine();

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::InputFloat("##VelocityZ", &(component.DeRef()->particleEmitter.initVelocity[2]));

				ImGui::Text("%s", "Random Velocity");

				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomVelocityX", &(component.DeRef()->particleEmitter.useRandomInitVelocityX));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomVelocityX", &(component.DeRef()->particleEmitter.minVelocityX));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomVelocityX", &(component.DeRef()->particleEmitter.maxVelocityX));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformInitVelocityX", &(component.DeRef()->particleEmitter.useUniformInitVelocityX));


				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomVelocityY", &(component.DeRef()->particleEmitter.useRandomInitVelocityY));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomVelocityY", &(component.DeRef()->particleEmitter.minVelocityY));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomVelocityY", &(component.DeRef()->particleEmitter.maxVelocityY));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformInitVelocityY", &(component.DeRef()->particleEmitter.useUniformInitVelocityY));

				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::Checkbox("##RandomVelocityZ", &(component.DeRef()->particleEmitter.useRandomInitVelocityZ));
				ImGui::SameLine();

				ImGui::Text("Min");
				ImGui::SameLine();
				ImGui::InputFloat("##MinRandomVelocityZ", &(component.DeRef()->particleEmitter.minVelocityZ));
				ImGui::SameLine();

				ImGui::Text("Max");
				ImGui::SameLine();
				ImGui::InputFloat("##MaxRandomVelocityZ", &(component.DeRef()->particleEmitter.maxVelocityZ));
				ImGui::SameLine();

				ImGui::Text("%s", "Uniform");
				ImGui::SameLine();
				ImGui::Checkbox("##UseUniformInitVelocityZ", &(component.DeRef()->particleEmitter.useUniformInitVelocityZ));

				ImGui::PopItemWidth();
			}
		}

		if (ImGui::CollapsingHeader("Shader Settings"))
		{
			auto ColourPickerFlags =
					ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar;

			ImGui::Text("Start Colour: ");
			ImGui::SameLine();
			ImVec4 startColour((component.DeRef()->startColour[0]),
					(component.DeRef()->startColour[1]),
					(component.DeRef()->startColour[2]),
					(component.DeRef()->startColour[3]));
			ImGui::PushItemWidth(20);
			if (ImGui::ColorButton("##ButtonStartColour", startColour))
			{
				ImGui::OpenPopup("##PickStartColour");
			}
			ImGui::PopItemWidth();
			if (ImGui::BeginPopup("##PickStartColour"))
			{
				ImGui::PushItemWidth(180.0f);
				ImGui::ColorPicker4("##StartColour", (float*)&(component.DeRef()->startColour),
						ColourPickerFlags);
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			ImGui::SameLine();
			ImGui::Text("End Colour: ");
			ImGui::SameLine();
			ImVec4 endColour((component.DeRef()->endColour[0]),
					(component.DeRef()->endColour[1]),
					(component.DeRef()->endColour[2]),
					(component.DeRef()->endColour[3]));
			ImGui::PushItemWidth(20);
			if (ImGui::ColorButton("##ButtonEndColour", endColour))
			{
				ImGui::OpenPopup("##PickEndColour");
			}
			ImGui::PopItemWidth();
			if (ImGui::BeginPopup("##PickEndColour"))
			{
				ImGui::PushItemWidth(180.0f);
				ImGui::ColorPicker4("##EndColour", (float*)&(component.DeRef()->endColour), ColourPickerFlags);
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			// Possible options to shade by
			const std::string items[] = { "Life", "Position", "Size", "Speed" };
			ImGui::Text("Shade by: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall - 30);
			ImGui::PushItemWidth(90);
			if (ImGui::BeginCombo("##ShadeByCombo", component.DeRef()->shadeBy.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (component.DeRef()->shadeBy == items[n]);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
						component.DeRef()->shadeBy = items[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			ImGui::Text("Min");
			float maxVal = 100.0f;
			if (component.DeRef()->shadeBy == "Life")
			{
				maxVal = component.DeRef()->particleEmitter.respawnLifetime;
				if (component.DeRef()->maxShadeValue > maxVal)
					component.DeRef()->maxShadeValue = maxVal;
				if (component.DeRef()->minShadeValue > maxVal)
					component.DeRef()->minShadeValue = maxVal;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::SliderFloat("##MinShadeValue", &component.DeRef()->minShadeValue, 0.f, maxVal, "%.3f",
					ImGuiSliderFlags_None);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text("Max");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::SliderFloat("##MaxShadeValue", &component.DeRef()->maxShadeValue, 0.f, maxVal, "%.3f",
					ImGuiSliderFlags_None);
			ImGui::PopItemWidth();

			const std::string scaleByItems[] = { "Constant", "Life", "Position", "Speed" };
			ImGui::Text("Scale by: ");
			ImGui::SameLine(_textWidthParticleEffectComponentSmall - 30);
			ImGui::PushItemWidth(90);
			if (ImGui::BeginCombo("##ScaleByCombo", component.DeRef()->particleEmitter.scaleBy.c_str()))
			{
				for (int n = 0; n < IM_ARRAYSIZE(scaleByItems); n++)
				{
					bool is_selected = (component.DeRef()->particleEmitter.scaleBy == scaleByItems[n]);
					if (ImGui::Selectable(scaleByItems[n].c_str(), is_selected))
						component.DeRef()->particleEmitter.scaleBy = scaleByItems[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			ImGui::Text("%s", "Scale Factor: ");
			ImGui::SameLine();
			ImGui::SliderFloat("##ScaleFactorParticles", &component.DeRef()->particleEmitter.scaleFactor, 0.f, 10.0f, "%.3f",
					ImGuiSliderFlags_None);

			char textureBuffer[64];
			if (component.DeRef()->particleEmitter.texture.DeRef() != nullptr)
				strcpy(textureBuffer, component.DeRef()->particleEmitter.texture.DeRef()->fileName.c_str());
			else
				memset(textureBuffer, 0, 64 * sizeof(char));
			ImGui::Text("%s", "Texture:");
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
				auto [success, asset] = _assetHelper->PickAssetGUIWindow<Texture>("Select Particle Effect Texture");
				if (success)
					component.DeRef()->particleEmitter.texture = asset;
			}
			ImGui::SameLine();
			ImGui::Checkbox("##UseParticleEffectTexture", &(component.DeRef()->useTexture));

			ImGui::Text("Number of Rows: ");
			ImGui::SameLine();
			ImGui::InputInt("##NumberOfRowsInTexture", &(component.DeRef()->particleEmitter.numRowsInTexture));
			ImGui::Text("Number of Columns: ");
			ImGui::SameLine();
			ImGui::InputInt("##NumberOfColsInTexture", &(component.DeRef()->particleEmitter.numColsInTexture));

			ImGui::Text("%s", "Use Cylinder billboard");
			ImGui::SameLine();
			ImGui::Checkbox("##Use Cylinder billboard", &(component.DeRef()->particleEmitter.useCylindricalBillboard));
		}
	}
}

void InspectorWindow::ShowAudioComponent(Scene& scene)
{
	auto obj = _sceneEditor->GetSelectedGameobject();
	SavedReference<AudioComponent> audioComponent = obj.DeRef()->GetComponent<AudioComponent>();
	if (!audioComponent)
		return;
	AudioComponent* audioComponentPointer = audioComponent.DeRef().get();

	ImGui::Separator();
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_TABLE_CELLS "  Audio", ImGuiTreeNodeFlags_AllowItemOverlap);
	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveAudioComponent"))
	{
		// remove component
		scene.RemoveComponent(audioComponent);
		return;
	}
	if (isHeaderOpen)
	{
		ImGui::Text("Audio");
		ImGui::SameLine();
		ImGui::PushItemWidth(130.0f);

		// store the current mesh name into mesh buffer, so that we can display it in the input text box
		std::string emptyString;
		std::string* audioClipName = nullptr;
		if (audioComponentPointer->audioClip)
			audioClipName = &audioComponentPointer->audioClip.DeRef()->fileName;
		else
			audioClipName = &emptyString;

		// show text box (read only)
		ImGui::InputText("##Sample Name", audioClipName->data(), audioClipName->size(), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		const char SELECT_AUDIO_CLIP[] = "Select Audio Clip";

		if (ImGui::Button("Choose sample"))
		{
			ImGui::OpenPopup(SELECT_AUDIO_CLIP);
		}

		if (ImGui::Button("Play"))
		{
			obj.DeRef()->GetComponent<AudioComponent>().DeRef()->Play();
		}

		auto [success, asset] = _assetHelper->PickAssetGUIWindow<AudioClip>(SELECT_AUDIO_CLIP);
		if (success)
		{
			obj.DeRef()->GetComponent<AudioComponent>().DeRef()->audioClip = asset;
		}
	}
}

void InspectorWindow::ShowAnimationComponent(Scene& scene)
{
	ImGui::Separator();
	char meshBuffer[64];
	char textureBuffer[64];
	bool isHeaderOpen = ImGui::CollapsingHeader(ICON_FA_TABLE_CELLS "  Animation", ImGuiTreeNodeFlags_AllowItemOverlap);
	auto obj = _sceneEditor->GetSelectedGameobject();

	// TODO: Icon button maybe?
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - 4.0f);
	if (ImGui::Button("X##RemoveAnimationComponent"))
	{
		// remove component
		scene.RemoveComponent(obj.DeRef()->GetComponent<AnimationComponent>());
		return;
	}
	if (isHeaderOpen)
	{

		// store pointer of renderComponent
		AnimationComponent* animationComponentPointer = obj.DeRef()->GetComponent<AnimationComponent>().DeRef().get();

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

void InspectorWindow::ShowAddComponent(Scene& scene)
{
	if (ImGui::BeginChild("ComponentSelector"))
	{
		auto& obj = _sceneEditor->GetSelectedGameobject();

		const char* components[] = {
				"Mesh Render Component",
				"Transform Component",
				"Camera Component",
				"RigidBody Component",
				"BoxCollider Component",
				"SphereCollider Component",
				"CapsuleCollider Component",
				"Particle Effect Component",
				"Audio Component",
				"Animation Component"
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
				scene.AddComponent<RigidBody>(obj);
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
			else if (strcmp(selectedComponent, "Animation Component") == 0)
			{
				scene.AddComponent<AnimationComponent>(obj);

				if(obj.DeRef()->GetComponent<MeshRender>())
				{
					obj.DeRef()->GetComponent<AnimationComponent>().DeRef()->SetMesh(obj.DeRef()->GetComponent<MeshRender>().DeRef()->GetMesh());
				}
			}
			_isAddComponentWindowOpen = false;
			selectedComponent = nullptr;
		}
	}
	ImGui::EndChild();
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