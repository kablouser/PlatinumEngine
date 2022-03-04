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

		// Render objects
		void RenderObjects(sf::RenderTexture& renderTexture, ImVec2 targetSize);

		// TODO receive light properties from scene editor
		void SetLightProperties();

	private:

		// true iff all init steps were successful
		bool _isInitGood;
		// settings for rendering OpenGL
		sf::ContextSettings _contextSettings;
		sf::Shader _unlitShader;
		ShaderInput _unlitShaderInput;

		// these two parameters are creating for testing the effect of Blinn-Phong-Shading
		sf::Clock clock;
		sf::Time time;

		// create a cube object to test for this module. I will delete after finishing the scene module.
		void CubeTest();
	};
}
