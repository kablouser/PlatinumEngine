//
// Created by Matt on 27/03/2022.
//

#pragma once

#include "WindowManager/Filedialog.h"

#include <ComponentComposition/Component.h>

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


#include <SceneManager/SceneManager.h>
#include <OpenGL/Mesh.h>
#include <SceneEditor/SceneEditor.h>
#include <AssetDatabase/AssetHelper.h>

#include <IconsFontAwesome6.h>
namespace PlatinumEngine
{
	class InspectorWindow
	{
	public:
		InspectorWindow(AssetHelper* assetHelper, SceneEditor* sceneEditor, Physics* physics);
		void ShowGUIWindow(bool* isOpen, Scene& scene);
	private:
		// TODO: Add specific component guis as components are created
		void ShowMeshRenderComponent(Scene& scene);
		void ShowTransformComponent(Scene& scene);
		void ShowCameraComponent(Scene& scene);
		void ShowRigidBodyComponent(Scene& scene);
		void ShowBoxColliderComponent(Scene& scene);
		void ShowSphereColliderComponent(Scene& scene);
		void ShowCapsuleColliderComponent(Scene& scene);
		void ShowParticleEffectComponent(Scene &scene);
		void ShowAudioComponent(Scene& scene);
		void ShowAnimationComponent(Scene& scene);


		// Shown when add component button pressed
		void ShowAddComponent(Scene& scene);

    	void cameraComponentHelper(char* cameraType[]);
    
		std::filesystem::path GetPayloadPath(const ImGuiPayload* payload);
	private:
		AssetHelper* _assetHelper;
		SceneEditor* _sceneEditor;
    	Physics* _physics;

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