//
// Created by Matt on 27/03/2022.
//

#pragma once

#include "WindowManager/Filedialog.h"

#include <ComponentComposition/Component.h>
#include <ComponentComposition/LightComponent.h>
#include <ComponentComposition/MeshRender.h>
#include <ComponentComposition/Transform.h>
#include <ComponentComposition/Camera.h>
#include <ComponentComposition/AnimationComponent.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/RigidBody.h>
#include <ComponentComposition/BoxCollider.h>
#include <ComponentComposition/CapsuleCollider.h>
#include <ComponentComposition/SphereCollider.h>
#include <ComponentComposition/AudioComponent.h>
#include <ComponentComposition/ParticleEffect.h>

#include <ParticleEffects/ParticleEmitter.h>

#include <OpenGL/Mesh.h>
#include <SceneEditor/SceneEditor.h>

#include <IconsFontAwesome6.h>
namespace PlatinumEngine
{
	class InspectorWindow
	{
	public:
		InspectorWindow();
		void ShowGUIWindow(bool* isOpen);
	private:
		// TODO: Add specific component guis as components are created
		void ShowLightComponent(SavedReference<LightComponent>& reference);
		void ShowMeshRenderComponent(SavedReference<MeshRender>& reference);
		void ShowTransformComponent(SavedReference<Transform>& reference);
		void ShowCameraComponent(SavedReference<Camera>& reference);
		void ShowRigidBodyComponent(SavedReference<RigidBody>& reference);
		void ShowBoxColliderComponent(SavedReference<BoxCollider>& reference);
		void ShowSphereColliderComponent(SavedReference<SphereCollider>& reference);
		void ShowCapsuleColliderComponent(SavedReference<CapsuleCollider>& reference);
		void ShowParticleEffectComponent(SavedReference<ParticleEffect>& reference);
		void ShowAudioComponent(SavedReference<AudioComponent>& reference);
		void ShowAnimationComponent(SavedReference<AnimationComponent>& reference);


		// Shown when add component button pressed
		void ShowAddComponent();

    	void cameraComponentHelper(char* cameraType[]);
    
		std::filesystem::path GetPayloadPath(const ImGuiPayload* payload);

	private:

		bool _isAddComponentWindowOpen = false;

		//ImGui helper parameters
		float _textWidth = 140.f;
		float _itemWidthMeshRenderComponent = 160.f;

		float _textWidthTransformComponent = 90.f;
		float _textWidthCameraComponent = 135.0f;
		float _textWidthParticleEffectComponent = 135.0f;
		float _textWidthParticleEffectComponentSmall = 100.0f;
		float _itemWidthParticleEffectComponent = 180.0f;

		std::vector<std::string> _temp = {"Perspective", "Orthographic"};
		std::vector<std::string> _clearMode = {"None", "SkyBox", "BackgroundColour"};
	};
}