//
// Created by Jason on 07/02/2022.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <OpenGL/ShaderInput.h>
#include <imgui.h>

// PlatinumEngine lib
#include <SceneEditor/EditorCamera.h>

namespace PlatinumEngine
{
	class RasterRenderer : sf::GlResource
	{

	public:

		// VARIABLE

		// Editor camera
		PlatinumEngine::EditorCamera camera;

		// Constructors
		RasterRenderer(
				const sf::Window& parentWindow,
				unsigned int depthBits = 24,
				unsigned int stencilBits =8,
				unsigned int antiAliasingLevel=4
				);

		~RasterRenderer();

		void RenderObjects(sf::RenderTexture& renderTexture, ImVec2 targetSize);

	private:

		// true iff all init steps were successful
		bool _isInitGood;
		// settings for rendering OpenGL
		sf::ContextSettings _contextSettings;
		sf::Shader _unlitShader;
		ShaderInput _unlitShaderInput;

	};
}
