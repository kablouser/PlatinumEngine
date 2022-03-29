//
// Created by shawn on 2022/3/29.
//

#include "GameWindow/GameWindow.h"

namespace PlatinumEngine
{
	PlatinumEngine::GameWindow::GameWindow(InputManager* inputManager, Scene* scene, Renderer* renderer)
	{

	}

	PlatinumEngine::GameWindow::GameWindow()
	{

	}
	/*
	void PlatinumEngine::GameWindow::Start(bool* OnStart)
	{
		if(OnStart)
		{
			PlatinumEngine::GameWindow::Update();
		}
	}
*/
	void PlatinumEngine::GameWindow::ShowGuiWindow(bool* outIsOpen)
	{
		if(ImGui::Begin("Game View", outIsOpen))
		{


			ImGui::End();
		}
	}
}