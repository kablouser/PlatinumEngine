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

int main()
{
	PlatinumEngine::RasterRenderer rasterRenderer;
	sf::Clock deltaClock;
	while(rasterRenderer.Update(deltaClock))
	{
		deltaClock.restart();
	}

	return 0;
}