//
// Created by Matt on 27/03/2022.
//

#pragma once

#include "WindowManager/Filedialog.h"

#include <ComponentComposition/Component.h>
#include <ComponentComposition/RenderComponent.h>
#include <ComponentComposition/TransformComponent.h>
#include <ComponentComposition/CameraComponent.h>
#include <ComponentComposition/AudioComponent.h>
#include <ComponentComposition/GameObject.h>
#include <ComponentComposition/ParticleEffect.h>

#include <ParticleEffects/ParticleEmitter.h>

#include <SceneManager/SceneManager.h>
#include <OpenGL/Mesh.h>
#include <SceneEditor/SceneEditor.h>

#include <IconsFontAwesome6.h>
namespace PlatinumEngine
{
	class InspectorWindow
	{
	public:
		InspectorWindow(AssetHelper* assetHelper, SceneEditor* sceneEditor);
		void ShowGUIWindow(bool* isOpen, Scene& scene);
	private:
		// TODO: Add specific component guis as components are created
		void ShowMeshRenderComponent(Scene& scene);
		void ShowTransformComponent(Scene& scene);
		void ShowCameraComponent(Scene& scene);
		void ShowParticleEffectComponent(Scene &scene);
		void ShowAudioComponent(Scene& scene);


		// Shown when add component button pressed
		void ShowAddComponent(Scene& scene);

		std::filesystem::path GetPayloadPath(const ImGuiPayload* payload);

		void cameraComponentHelper(char* cameraType[]);
	private:
		AssetHelper* _assetHelper;
		SceneEditor* _sceneEditor;
		bool _isAddComponentWindowOpen = false;

		// Have to keep track of if object enabled ourselves as isEnabled is a private member of game object
		bool _isObjectEnabled;

		//ImGui helper parameters
		float _textWidthMeshRenderComponent = 90.f;
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