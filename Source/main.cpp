#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "InputManager/InputManager.h"
#include "RasterRenderer/RasterRenderer.h"
#include "SceneManager/SceneManager.h"
#include "WindowManager/WindowManager.h"
int main()
{
	// TODO
	// Do stuff ...
	PlatinumEngine::InputManager inputManager;
	PlatinumEngine::RasterRenderer rasterRenderer;
	PlatinumEngine::SceneManager sceneManager;

  PlatinumEngine::WindowManager windowManager;
	Assimp::Importer import;
	// assimp!
	const aiScene* scene = import.ReadFile("Test", aiProcess_Triangulate | aiProcess_FlipUVs);
  
	// GLM!
	glm::vec4 Position = glm::vec4(glm::vec3(0.0), 1.0);
	glm::mat4 Model = glm::mat4(1.0);
	Model[3] = glm::vec4(1.0, 1.0, 0.0, 1.0);
	glm::vec4 Transformed = Model * Position;

	sf::RenderWindow window(sf::VideoMode(800, 800), "Window Title");
	ImGui::SFML::Init(window);

	bool circleExists = true;
	float circleRadius = 200.0f;
	int circleSegments = 100;
	float circleColor[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
	sf::CircleShape shape(circleRadius, circleSegments);
	shape.setFillColor(sf::Color
			(
					(int)(circleColor[0] * 255),
					(int)(circleColor[1] * 255),
					(int)(circleColor[2] * 255)
			)); // Color circle
	shape.setOrigin(sf::Vector2f(circleRadius, circleRadius));
	shape.setPosition(sf::Vector2f(400, 400)); // Center circle

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Window title");
		ImGui::Text("Window text!");
		ImGui::Checkbox("Circle", &circleExists);
		ImGui::SliderFloat("Radius", &circleRadius, 100.0f, 300.0f);
		ImGui::SliderInt("Sides", &circleSegments, 3, 150);
		ImGui::ColorEdit3("Color Circle", circleColor);
		ImGui::End();

		shape.setRadius(circleRadius);
		shape.setOrigin(sf::Vector2f(circleRadius, circleRadius));
		shape.setPointCount(circleSegments);
		shape.setFillColor(sf::Color
				(
						(int)(circleColor[0] * 255),
						(int)(circleColor[1] * 255),
						(int)(circleColor[2] * 255)
				)); // Color circle

		window.clear(sf::Color(18, 33, 43)); // Color background
		if (circleExists)
			window.draw(shape);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
  
	return 0;
}