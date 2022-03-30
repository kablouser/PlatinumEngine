//
// Created by shawn on 2022/3/29.
//

#include "GameWindow/GameWindow.h"
#include <ComponentComposition/RenderComponent.h>
namespace PlatinumEngine
{
	PlatinumEngine::GameWindow::GameWindow()
	{

	}

	PlatinumEngine::GameWindow::GameWindow(InputManager* inputManager, Scene* scene, Renderer* renderer): _renderer(renderer), _scene(scene), _inputManager(inputManager),_renderTexture()
	{
		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_renderTexture.Create(_framebufferWidth, _framebufferHeight))
			return;
	}

	void PlatinumEngine::GameWindow::ShowGuiWindow(bool* outIsOpen)
	{
		if(ImGui::Begin("Game View", outIsOpen))
		{
			auto targetSize = ImGui::GetContentRegionAvail();
			// TODO: add the user input detection(检测用户输入)
			if(_onUpdate == true)
			{
				Update();
			}
			Render(targetSize, _scene);
			ImGui::End();
		}
	}

	void PlatinumEngine::GameWindow::Pause(bool OnPause)
	{
		// TODO: stop the update function(暂停游戏)
		if(OnPause == true)
		{
			std::cout << "continue" << std::endl;
		}
		else if(OnPause == false)
		{
			std::cout << "pause" << std::endl;
		}
	}

	void PlatinumEngine::GameWindow::Stop()
	{

	}

	void GameWindow::Render(ImVec2 targetSize, Scene* scene)
	{
		_framebufferWidth = (int)targetSize.x;
		_framebufferHeight = (int)targetSize.y;
		_renderTexture.Create(_framebufferWidth, _framebufferHeight);
		if(1.0f < targetSize.x && 1.0f < targetSize.y)
		{
			// resize framebuffer if necessary
			if (_framebufferWidth != (int)targetSize.x || _framebufferHeight != (int)targetSize.y)
			{
				_framebufferWidth = (int)targetSize.x;
				_framebufferHeight = (int)targetSize.y;
				_renderTexture.Create(_framebufferWidth, _framebufferHeight);
			}

			_renderTexture.Bind();
			glEnable(GL_DEPTH_TEST);
			glViewport(0, 0, _framebufferWidth, _framebufferHeight);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//			Mesh mesh(vertices, indices);
			_renderer->Begin();
			scene->Render(*_renderer);
			_renderer->End();

			_renderTexture.Unbind();
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			// display updated framebuffer
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize);
		}
	}

	void PlatinumEngine::GameWindow::Update()
	{
			double newTime = glfwGetTime();
			double deltaTime = newTime - _previousTime;
			_scene->Update(deltaTime);
			_previousTime = newTime;
	}


}