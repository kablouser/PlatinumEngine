//
// Created by Shihua on 02/03/2022.
//

#pragma once

// Platinum Library
#include <Renderer/Renderer.h>
#include <SceneEditor/EditorCamera.h>
#include <SceneEditor/InputManagerExtend.h>
#include <SceneManager/Scene.h>
#include <OpenGL/Framebuffer.h>

namespace PlatinumEngine
{
	class SceneEditor
	{
	public:
		// ___VARIABLE___

		// ___FUNCTION___

		/**
		 * Show scene editor window
		 * @param outIsOpen : flag for showing this window or not
		 * @param rasterRenderer :  Reference of the engine renderer
		 */
		void ShowGUIWindow(bool* outIsOpen);

		/**
		 * Update data in Scene Editor
		 */
		void Update(ImVec2 targetSize);

		// ___CONSTRUCTOR___
		SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer);

		// ___DESTRUCTOR___
		//~SceneEditor();

	private:

		// ___PARAMETERS___

		// Flags
		bool _ifCameraSettingWindowOpen;

		// Managers
		InputManager* _inputManager;
		Scene* _scene;
		Renderer* _renderer;

		// Values for Camera
		EditorCamera _camera;
		int _fov;
		int _near;
		int _far;

		ImVec2 _mouseMoveDelta;
		int _mouseButtonType;
		float _wheelValueDelta;


		// output of OpenGL rendering
		Framebuffer _renderTexture;
	};
}