#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <InputManager/InputManager.h>
#include <RasterRenderer/RasterRenderer.h>
#include <SceneEditor/SceneEditor.h>
#include <WindowManager/WindowManager.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 960), "Platinum Engine");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	PlatinumEngine::RasterRenderer rasterRenderer(window);
	bool isRasterRendererOpen = true;
	PlatinumEngine::InputManager inputManager;
	bool isInputWindowOpen = true;
	PlatinumEngine::WindowManager windowManager;

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			windowManager.DoShortCut(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
		}
		// if we don't break now, trying to use OpenGL might cause crashes
		if(!window.isOpen())
			break;
		ImGui::SFML::Update(window, deltaClock.restart());

		// Background color of main window
		window.clear(sf::Color(18, 33, 43));


		//--------------------------------------------------------------------------------------------------------------
		// GUI HERE
		//--------------------------------------------------------------------------------------------------------------
//		if(isRasterRendererOpen)
//			rasterRenderer.ShowGUIWindow(&isRasterRendererOpen);
		if(isInputWindowOpen)
			inputManager.ShowGUIWindow(&isInputWindowOpen);

		windowManager.ShowGUI();
		//--------------------------------------------------------------------------------------------------------------
		// END OF GUI
		//--------------------------------------------------------------------------------------------------------------


		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();


	return 0;
}