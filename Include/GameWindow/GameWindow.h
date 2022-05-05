//
// Created by shawn on 2022/3/29.
//

#pragma once
#include <OpenGL/ShaderInput.h>
#include <SceneManager/Scene.h>
#include <Helpers/Time.h>
#include <Renderer/Renderer.h>
#include <Physics/Physics.h>

namespace PlatinumEngine
{
	class GameWindow
	{
	public:
		GameWindow(Scene* scene, Renderer* renderer, Time* time, Physics* physics);

		/**
		 * This is a Gui
		 * @param OutIsOpen
		 */
		void ShowGUIWindow(bool* OutIsOpen);

		bool GetIsStarted() const;
		void SetIsStarted(bool isStarted);

		/**
		 * If game is started, pauses game and updates the scene.
		 * Does nothing if the game is ended.
		 */
		void Step();

		/**
		 * Game state definition:
		 *
		 * Game can be started or ended. Default is ended.
		 *
		 * A started game can be paused or resumed.
		 * A ended game can neither be paused nor resumed.
		 * By default, a game is resumed.
		 *
		 * Scene.OnStart is called when game enters started state.
		 * Scene.OnEnd is called when game enters ended state.
		 * So, 1 Scene.OnStart always corresponds to 1 Scene.OnEnd.
		 *
		 * A paused game can be paused again. (no effect)
		 * A resumed game can be resumed again. (no effect)
		 *
		 * Scene.OnUpdate is called when a game is started and resumed.
		 */

		bool isPaused;

	private:
		bool _isStarted;

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
		bool _hasWarningBeenShown = false;

		/**
		 * Update the scene and physics by current delta time in _time object.
		 */
		void Update();

		/**
		 * Render the GameObjects in the scene
		 * @param targetSize
		 * @param scene
		 */
		void Render(ImVec2 targetSize);

		void CreateSkyBoxShaderInput();
	};
}