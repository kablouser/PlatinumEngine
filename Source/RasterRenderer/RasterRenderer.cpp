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
				sf::ContextSettings(depthBits, stencilBits, antialiasingLevel)),
		camera()
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
		if(event.type == sf::Event::MouseButtonPressed)
		{
			if(event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i newMousePosition =sf::Mouse::getPosition(_window);

				camera.isFirstMousePress = true;
				camera.UpdateInitialMousePosition(glm::vec2(newMousePosition.x, newMousePosition.y));

			}

		}
		else if(event.type == sf::Event::MouseMoved)
		{

			if(camera.isFirstMousePress == false)
				continue;

			sf::Vector2i newMousePosition = sf::Mouse::getPosition(_window);
			camera.RotationByMouse(glm::vec2(newMousePosition.x, newMousePosition.y));


		}
		else if(event.type == sf::Event::MouseButtonReleased)
		{
			if(event.mouseButton.button == sf::Mouse::Left)
			{

				camera.isFirstMousePress = false;


			}
		}
		else if(event.type == sf::Event::Closed)
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


	// text camera control
	camera.MoveCamera(glm::vec3(0.0,0.0,0),glm::vec3(0.0,0.0,0.0));

	glm::vec4 vertex0(-0.5f, -0.5f, -0.1f, 1.0);
	glm::vec4 vertex1(0.5f, -0.5f, -0.1f, 1.0);
	glm::vec4 vertex2(0.0f, 0.0f, -0.1f, 1.0);

	vertex0 = camera.lookAtMatrix4 * vertex0;
	vertex1 = camera.lookAtMatrix4 * vertex1;
	vertex2 = camera.lookAtMatrix4 * vertex2;

	std::cout<<vertex2.z<<std::endl;


	glVertex3f(vertex0.x/vertex0.w, vertex0.y/vertex0.w, vertex0.z/vertex0.w);
	glVertex3f(vertex1.x/vertex1.w, vertex1.y/vertex1.w, vertex1.z/vertex1.w);
	glVertex3f(vertex2.x/vertex2.w, vertex2.y/vertex2.w, vertex2.z/vertex2.w);




	glEnd();


	// display window
	ImGui::SFML::Render(_window);

	_window.display();
	return true;

}

void RasterRenderer::CreateShader()
{

	GLfloat vertices[]=
			{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.0f, 0.0f

			};

	if (glewInit() == GLEW_OK)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 10);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glUseProgram(0);


	}


}