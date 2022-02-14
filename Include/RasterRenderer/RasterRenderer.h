//
// Created by Jason on 07/02/2022.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <OpenGL/ShaderInput.h>

// PlatinumEngine lib
#include "EditorCamera.h"

namespace PlatinumEngine
{
	class RasterRenderer : sf::GlResource
	{

	public:
		// VARIABLE
		PlatinumEngine::EditorCamera camera;


		// Functions

		/**
		 * Update()
		 * update content inside the window.
		 * @return
		 * true:  if window is open and display successfully
		 * false: if window is not open or cannot display content.
		 */
		bool Update(const sf::Clock& deltaClock);

		// Constructors
		RasterRenderer(
				unsigned int depthBits = 24,
				unsigned int stencilBits =8,
				unsigned int antialiasingLevel=4,
				unsigned int width =800,
				unsigned int height=600
				);

		~RasterRenderer();

	private:

		// Parameters
		sf::RenderWindow _renderWindow;

		// true iff all init steps were successful
		bool _isInitGood;
		sf::Shader _unlitShader;
		ShaderInput _unlitShaderInput;
	};
}
