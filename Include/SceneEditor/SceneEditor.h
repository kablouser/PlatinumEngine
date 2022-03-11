//
// Created by Shihua on 02/03/2022.
//

#pragma once

#include <Renderer/Renderer.h>
#include "Renderer/Renderer.h"
#include "EditorCamera.h"
#include "SceneEditor/InputManagerExtend.h"

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
		void ShowGUIWindow(bool* outIsOpen, Renderer& rasterRenderer);

		/**
		 * Update data in Scene Editor
		 */
		void Update();




		// ___CONSTRUCTOR___
		SceneEditor(InputManagerExtend* inputManager);//, Scene* scene);



		// ___DESTRUCTOR___
		//~SceneEditor();

	private:

		// ___PARAMETERS___

		// Flags
		bool _ifCameraSettingWindowOpen;

		// Input Manager
		InputManagerExtend* _inputManager;
		//Scene* _scene;

		// Values for Camera
		EditorCamera _camera;
		int _fov;
		int _near;
		int _far;

		ImVec2 _mouseMoveDelta;
		InputManagerExtend::MouseButtonType _mouseButtonType;
		float _wheelValueDelta;

		// intermediate output of OpenGL rendering
		// sf::RenderTexture _renderTexture;



	};
}