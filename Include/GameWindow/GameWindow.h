//
// Created by shawn on 2022/3/29.
//

#pragma once
#include "InputManager/InputManager.h"
#include "OpenGL/ShaderInput.h"
#include "SceneManager/Scene.h"
#include <Helpers/Time.h>
#include "Renderer/Renderer.h"
#include <Physics/Physics.h>

namespace PlatinumEngine
{
	class GameWindow
	{
	public:
		GameWindow(InputManager* inputManager, Scene* scene, Renderer* renderer, Time* time, Physics* physics);
		GameWindow();
		//~GameWindow();

		/**
		 * This is a Gui
		 * @param OutIsOpen
		 */
		void ShowGuiWindow(bool* OutIsOpen);

		/**
		 * Pause function that will make the update in gameWinodw pause
		 */
		void Pause();

		/**
		 * Step function for updating the scene
		 */
		void Step();

		/**
		 * Start the simulation
		 */
		void Play();

		/**
		 * Render the GameObject states in scene
		 */
		void Update(double deltaTime);

		/**
		 * Render the GameObjects in the scene
		 * @param targetSize
		 * @param scene
		 */
		void Render(ImVec2 targetSize, Scene* scene);


		void CreateSkyBoxShaderInput();

	private:
		InputManager* _inputManager;
		Scene* _scene;
		Renderer* _renderer;
		Time* _time;
		Physics* _physics;
		// Skybox
		Texture _skyboxTexture;
		ShaderInput _skyBoxShaderInput;

		// output of OpenGL rendering
		Framebuffer _renderTexture;
		int _framebufferWidth;
		int _framebufferHeight;

		// user event
		ImVec2 _mouseMoveDelta;
		int _mouseButtonType;
		float _wheelValueDelta;

		bool _isPlay;
		bool _hasWarningBeenShown = false;
	public:
		bool isPaused;
	};
}