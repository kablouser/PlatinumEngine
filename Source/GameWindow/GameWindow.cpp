//
// Created by shawn on 2022/3/29.
//

#include "GameWindow/GameWindow.h"
#include <ComponentComposition/RenderComponent.h>
#include <ComponentComposition/CameraComponent.h>
#include <IconsFontAwesome6.h>
namespace PlatinumEngine
{
	PlatinumEngine::GameWindow::GameWindow()
	{

	}

	PlatinumEngine::GameWindow::GameWindow(InputManager* inputManager, Scene* scene, Renderer* renderer)
		: _renderer(renderer), _scene(scene), _inputManager(inputManager),_renderTexture()
	{
		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_renderTexture.Create(_framebufferWidth, _framebufferHeight))
			return;
	}

	void PlatinumEngine::GameWindow::ShowGuiWindow(bool* outIsOpen)
	{
		if(ImGui::Begin(ICON_FA_GAMEPAD " Game View", outIsOpen))
		{
			auto targetSize = ImGui::GetContentRegionAvail();

			if(_onUpdate && _onPlay)
			{
				//_mouseButtonType = _inputManager->GetMouseDown();
				//_mouseMoveDelta = _inputManager->GetMouseMoveVector();
				//_wheelValueDelta = _inputManager->GetMouseWheelDeltaValue();
				double deltaTime = glfwGetTime() - _previousTime;
				Update(deltaTime);
			}
			Render(targetSize, _scene);
		}
    
		ImGui::End();
	}

	void PlatinumEngine::GameWindow::Pause(bool OnPause)
	{
		_onPlay = !OnPause;
	}

	void PlatinumEngine::GameWindow::Step()
	{
		_onPlay = false;
		Update(1.0 / 30.0);
	}

	void GameWindow::Render(ImVec2 targetSize, Scene* scene)
	{
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
			// must begin renderer before matrices are set
			_renderer->Begin();
			auto camera = _scene->FindFirstComponent<CameraComponent>();
			if (camera)
			{
				// success so allow warning to be shown for next time
				_hasWarningBeenShown = false;

				glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, _framebufferWidth, _framebufferHeight);

				switch (camera.pointer->clearMode)
				{
				case CameraComponent::ClearMode::skybox:
					break;
				case CameraComponent::ClearMode::backgroundColor:
					glClearColor(
							camera.pointer->backgroundColor.r,
							camera.pointer->backgroundColor.g,
							camera.pointer->backgroundColor.b,
							camera.pointer->backgroundColor.a);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
					break;
				case CameraComponent::ClearMode::none:
					break;
				}

				_renderer->SetViewMatrix(camera.pointer->GetViewMatrix());
				_renderer->SetProjectionMatrix(camera.pointer->GetProjectionMatrix(
						{(float)_framebufferWidth, (float)_framebufferHeight}));
			}
			else
			{
				if (!_hasWarningBeenShown)
				{
					PLATINUM_WARNING("Scene has no camera component. Cannot render anything.");
					_hasWarningBeenShown = true;
				}
			}

			scene->Render(*_renderer);
			_renderer->End();

			_renderTexture.Unbind();
			glDisable(GL_DEPTH_TEST);


			// display updated framebuffer
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize, ImVec2(0, 1), ImVec2(1, 0));
		}
	}

	void PlatinumEngine::GameWindow::Update(double deltaTime)
	{
		// check if there is any mouse input
		switch(_mouseButtonType)
		{
		case 0: break;
		case 1: break;
		case 2: break;
		default: break;
		}

		// check if there is any wheel input
		if (_wheelValueDelta != 0) {}

		// check if there is any keyboard input
		if (_inputManager->IsKeyPressed(GLFW_KEY_UP) || _inputManager->IsKeyPressed(GLFW_KEY_DOWN) ||
			_inputManager->IsKeyPressed(GLFW_KEY_LEFT) || _inputManager->IsKeyPressed(GLFW_KEY_RIGHT))
		{}

		_scene->Update(deltaTime);
		_previousTime += deltaTime;
	}

}