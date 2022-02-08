//
// Created by Jason on 07/02/2022.
//

// SFML
#include "RasterRenderer/RasterRenderer.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>



// std
#include <iostream>

using namespace PlatinumEngine;

RasterRenderer::RasterRenderer(
		unsigned int depthBits,
		unsigned int stencilBits,
		unsigned int antialiasingLevel,
		unsigned int width,
		unsigned int height
		):
		_window(sf::VideoMode(width, height),
				"OpenGL",
				sf::Style::Default,
				sf::ContextSettings(depthBits, stencilBits, antialiasingLevel))
{

	_window.setVerticalSyncEnabled(true);

	// Debug
	sf::ContextSettings settings = _window.getSettings();
	std::cout << "depth bits:" << settings.depthBits << std::endl;
	std::cout << "stencil bits:" << settings.stencilBits << std::endl;
	std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
	std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

	// Enable ImGui in SFML
	ImGui::SFML::Init(_window);

}


RasterRenderer::~RasterRenderer()
{

	ImGui::SFML::Shutdown(_window);
}


bool RasterRenderer::Update(const sf::Clock& deltaClock)
{

	// if window is not open, return false
	if (!_window.isOpen())
		return false;

	// handle event
	sf::Event event;

	// loop through the events in the stacks
	while(_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		// if window is closed, stop the loop
		if(event.type == sf::Event::Closed)
		{

			_window.close();
			return false;
		}

	}

	// update imgui window
	ImGui::SFML::Update(_window, deltaClock.getElapsedTime());

	ImGui::Begin("Window title");
	ImGui::End();

	// opengl
	_window.clear(sf::Color(18,33,43));

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();


	// display window
	ImGui::SFML::Render(_window);

	_window.display();
	return true;

}

void RasterRenderer::CreateShader()
{
	if (glewInit() != GLEW_OK)
	{


	}


}