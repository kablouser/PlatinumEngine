//
// Created by Jason on 07/02/2022.
//

#pragma once

#include <SFML/Graphics.hpp>



namespace PlatinumEngine
{
	class RasterRenderer
	{

	public:
		// Functions
		/**
		 * Update()
		 * update content inside the window.
		 * @return
		 * true:  if window is open and display successfully
		 * false: if window is not open or cannot display content.
		 */
		bool Update(const sf::Clock& deltaClock);
		/**
		 * CreateShader()
		 * test with glew lib. have no valid code yet
		 * @return
		 * void
		 */
		void CreateShader();

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
		sf::RenderWindow _window;

	};
}
