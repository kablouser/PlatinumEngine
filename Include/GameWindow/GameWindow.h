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

		void ShowGuiWindow(bool* OutIsOpen);

		void Start(bool* OnStart);

		void Pause(bool* OnPause);

		void End(bool* OnEnd);

		void Update();
	private:
		InputManager* _inputManager;
		Scene* _scene;
		Renderer* _renderer;

		bool OnStart, OnPause, OnEnd = false;
	};
}