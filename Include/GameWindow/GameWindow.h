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

		/**
		 * This is a Gui
		 * @param OutIsOpen
		 */
		void ShowGuiWindow(bool* OutIsOpen);

		/**
		 * Pause function that will make the update in gameWinodw pause
		 * @param OnPause
		 */
		void Pause(bool OnPause);

		void Stop();

		/**
		 * Update the GameObject states in scene
		 */
		void Update();

		/**
		 * Render the GameObjects in the scene
		 * @param targetSize
		 * @param scene
		 */
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