//
// Created by Shihua on 02/03/2022.
//

#pragma once

#include <Renderer/Renderer.h>
#include "RasterRenderer/Renderer.h"
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
		SceneEditor(unsigned int depthBits = 24,
					unsigned int stencilBits =8,
					unsigned int antiAliasingLevel=4);



		// ___DESTRUCTOR___
		//~SceneEditor();

	private:

		// ___PARAMETERS___

		// Flags
		bool _ifCameraSettingWindowOpen;

		// Input Manager
		InputManagerExtend _inputManager;

		// Values for Camera
		EditorCamera _camera;
		int _fov;
		int _near;
		int _far;



		// settings for rendering OpenGL
		// sf::ContextSettings _contextSettings;

		// intermediate output of OpenGL rendering
		// sf::RenderTexture _renderTexture;



	};
}