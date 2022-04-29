//
// Created by shawn on 2022/3/29.
//

#include "GameWindow/GameWindow.h"
#include <ComponentComposition/MeshRender.h>
#include <ComponentComposition/Camera.h>


namespace PlatinumEngine
{
	PlatinumEngine::GameWindow::GameWindow()
	{

	}

	PlatinumEngine::GameWindow::GameWindow(InputManager* inputManager, Scene* scene, Renderer* renderer, Time* time, Physics* physics)
		: _renderer(renderer), _scene(scene), _inputManager(inputManager),_time(time), _physics(physics), _renderTexture(),	_skyboxTexture(),
			_skyBoxShaderInput(), isPaused(true), _isPlay(true)
	{
		// Setup skybox texture

		_skyboxTexture.CreateCubeMap({"./Assets/Texture/Left_X.png",
									  "./Assets/Texture/Right_X.png",
									  "./Assets/Texture/Up_Y.png",
									  "./Assets/Texture/Bottom_Y.png",
									  "./Assets/Texture/Front_Z.png",
									  "./Assets/Texture/Back_Z.png"});
		// Setup skybox texture shader input
		CreateSkyBoxShaderInput();

		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_renderTexture.Create(_framebufferWidth, _framebufferHeight))
			return;
	}

	void GameWindow::ShowGuiWindow(bool* outIsOpen)
	{
		if(ImGui::Begin(ICON_FA_GAMEPAD " Game View", outIsOpen))
		{
			auto targetSize = ImGui::GetContentRegionAvail();

			if(!isPaused)
				Play();
			Render(targetSize, _scene);
		}
		ImGui::End();
	}

	void GameWindow::Step()
	{
		isPaused = true;
		Update(_time->GetDelta());
	}

	void GameWindow::Pause()
	{
		isPaused = true;
	}

	void GameWindow::Play()
	{
		isPaused = false;
		Update(_time->GetDelta());
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
		_physics->Update(deltaTime);
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
			auto camera = _scene->FindFirstComponent<Camera>();
			if (camera)
			{
				// success so allow warning to be shown for next time
				_hasWarningBeenShown = false;

				glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, _framebufferWidth, _framebufferHeight);
				glViewport(0, 0, _framebufferWidth, _framebufferHeight);
				glClearColor(0.2784f, 0.2784f, 0.2784f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				Maths::Mat4 scaleMatrix;
				switch (camera->clearMode)
				{
				case Camera::ClearMode::skybox:
					// ---------------- Render SKY BOX ---------------- //
					// discard the depth of the skybox
					glDisable(GL_DEPTH_TEST);
					glDepthMask(false);
					_renderer->BeginSkyBoxShader();
					// matrix for rescaling the skybox based on the near panel distance
					scaleMatrix.SetScaleMatrix(
							Maths::Vec3(((float)camera->nearClippingPlane * 2.f), ((float)camera->nearClippingPlane * 2.f), ((float)camera->nearClippingPlane * 2.f)));
					// set matrix uniform
					_renderer->SetViewMatrixSkyBox(camera->GetViewMatrixRotationOnly()* scaleMatrix);
					_renderer->SetProjectionMatrixSkyBox(camera->GetProjectionMatrix(Maths::Vec2((float)_framebufferWidth,(float)_framebufferHeight)));
					_skyboxTexture.BindCubeMap();
					_skyBoxShaderInput.Draw();
					_renderer->EndSkyBoxShader();
					_skyboxTexture.UnbindCubeMap();
					_renderer->Begin();
					// enable depth test for the later rendering
					glDepthMask(true);
					glEnable(GL_DEPTH_TEST);
					break;
				case Camera::ClearMode::backgroundColor:
					glClearColor(
							camera->backgroundColor.r,
							camera->backgroundColor.g,
							camera->backgroundColor.b,
							camera->backgroundColor.a);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
					break;
				case Camera::ClearMode::none:
					break;
				}

				_renderer->SetViewMatrix(camera->GetViewMatrix());
				_renderer->SetProjectionMatrix(camera->GetProjectionMatrix(
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

			// unbind framebuffer
			_renderTexture.Unbind();

			// reset setting after rendering
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			// display updated framebuffer
			ImGui::Image(_renderTexture.GetColorTexture().GetImGuiHandle(), targetSize, ImVec2(0, 1), ImVec2(1, 0));
		}
	}

	///--------------------------------------------
	/// SkyBox
	///--------------------------------------------
	void GameWindow::CreateSkyBoxShaderInput()
	{

		std::vector<Vertex> skyboxVertices = {
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f,  1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},

				{Maths::Vec3(-1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f, -1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(-1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
				{Maths::Vec3(1.0f, -1.0f,  1.0f),Maths::Vec3(),Maths::Vec2()},
		};

		std::vector<GLuint> indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
									   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
									   30, 31, 32, 33, 34, 35};


		_skyBoxShaderInput.Set(skyboxVertices, indices);
	}

}