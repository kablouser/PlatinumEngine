//
// Created by Shihua on 02/03/2022.
//

#pragma once


//#include <SFML/Graphics.hpp>
#include <Renderer/Renderer.h>

namespace PlatinumEngine
{
	class SceneEditor
	{
	public:
		// ___VARIABLE___




		// ___FUNCTION___

		/// Show scene editor window
		/// \param outIsOpen: flag for showing this window or not
		void ShowGUIWindow(bool* outIsOpen, Renderer& rasterRenderer);



		// ___CONSTRUCTOR___
		SceneEditor(unsigned int depthBits = 24,
					unsigned int stencilBits =8,
					unsigned int antiAliasingLevel=4);



		// ___DESTRUCTOR___
		//~SceneEditor();

	private:
		// settings for rendering OpenGL
//		sf::ContextSettings _contextSettings;

		// intermediate output of OpenGL rendering
//		sf::RenderTexture _renderTexture;

	};
}