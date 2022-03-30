//
// Created by shawn on 2022/3/29.
//

#pragma once
#include "InputManager/InputManager.h"
#include "SceneManager/Scene.h"
#include "Renderer/Renderer.h"
namespace PlatinumEngine
{
	class GameWindow
	{
	public:
		GameWindow(InputManager* inputManager, Scene* scene, Renderer* renderer);
		GameWindow();
		//~GameWindow();

		void ShowGuiWindow(bool* OutIsOpen);

		void Pause(bool OnPause);

		void Stop();

		void Update();

		void Render(ImVec2 targetSize, Scene* scene);
	private:
		InputManager* _inputManager;
		Scene* _scene;
		Renderer* _renderer;

		// output of OpenGL rendering
		Framebuffer _renderTexture;
		int _framebufferWidth;
		int _framebufferHeight;

		double _previousTime = 0.0;

	public:
		bool _onUpdate = false;
	};
}