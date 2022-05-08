//
// Created by shawn on 2022/3/29.
//

#include <GameWindow/GameWindow.h>
#include <ComponentComposition/MeshRender.h>
#include <ComponentComposition/Camera.h>
#include <IconsFontAwesome6.h>
#include <Application.h>

namespace PlatinumEngine
{
	PlatinumEngine::GameWindow::GameWindow()
			: _renderTexture(), _skyboxTexture(),
			  _skyBoxShaderInput(), isPaused(false)
	{
		// Setup skybox texture
		_skyboxTexture.CreateCubeMap({ "./Assets/Texture/Left_X.png",
									   "./Assets/Texture/Right_X.png",
									   "./Assets/Texture/Up_Y.png",
									   "./Assets/Texture/Bottom_Y.png",
									   "./Assets/Texture/Front_Z.png",
									   "./Assets/Texture/Back_Z.png" });
		// Setup skybox texture shader input
		CreateSkyBoxShaderInput();

		_framebufferWidth = 1;
		_framebufferHeight = 1;
		if (!_renderTexture.Create(_framebufferWidth, _framebufferHeight))
			return;
	}

	void GameWindow::ShowGUIWindow(bool* outIsOpen)
	{
		if (Application::Instance->scene.IsStarted() && !isPaused)
			Update();

		if (ImGui::Begin(ICON_FA_GAMEPAD " Game View", outIsOpen))
		{
			auto targetSize = ImGui::GetContentRegionAvail();
			Render(targetSize);
		}
		ImGui::End();
	}

	bool GameWindow::GetIsStarted() const
	{
		return Application::Instance->scene.IsStarted();
	}

	void GameWindow::SetIsStarted(bool isStarted)
	{
		if (Application::Instance->scene.IsStarted() == isStarted)
			return; // this check is necessary

		if (isStarted)
			Application::Instance->scene.Start();
		else
			Application::Instance->scene.End();
	}

	void GameWindow::Step()
	{
		if (!Application::Instance->scene.IsStarted())
			return;

		isPaused = true;
		Update();
	}

	void PlatinumEngine::GameWindow::Update()
	{
		double deltaTime = Application::Instance->time.GetDelta();
		Application::Instance->scene.Update(deltaTime);
		// Physics must be after scene.
		Application::Instance->physics.Update(deltaTime);
	}

	void GameWindow::Render(ImVec2 targetSize)
	{
		if (1.0f < targetSize.x && 1.0f < targetSize.y)
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
			Application::Instance->renderer.Begin();
			auto camera = Application::Instance->scene.FindFirstComponent<Camera>();
			if (camera)
			{
				// success so allow warning to be shown for next time
				_hasWarningBeenShown = false;

				glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, _framebufferWidth, _framebufferHeight);
				glClearColor(0.2784f, 0.2784f, 0.2784f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				Maths::Mat4 scaleMatrix;
				switch (camera.DeRef()->clearMode)
				{
				case Camera::ClearMode::skybox:
					// ---------------- Render SKY BOX ---------------- //
					// discard the depth of the skybox
					glDisable(GL_DEPTH_TEST);
					glDepthMask(false);
					Application::Instance->renderer.BeginSkyBoxShader();
					// matrix for rescaling the skybox based on the near panel distance
					scaleMatrix.SetScaleMatrix(
							Maths::Vec3(((float)camera.DeRef()->nearClippingPlane * 2.f),
									((float)camera.DeRef()->nearClippingPlane * 2.f),
									((float)camera.DeRef()->nearClippingPlane * 2.f)));
					// set matrix uniform
					Application::Instance->renderer.SetViewMatrixSkyBox(camera.DeRef()->GetViewMatrixRotationOnly() * scaleMatrix);
					Application::Instance->renderer.SetProjectionMatrixSkyBox(camera.DeRef()->GetProjectionMatrix(
							Maths::Vec2((float)_framebufferWidth, (float)_framebufferHeight)));
					_skyboxTexture.BindCubeMap();
					_skyBoxShaderInput.Draw();
					Application::Instance->renderer.EndSkyBoxShader();
					_skyboxTexture.UnbindCubeMap();
					Application::Instance->renderer.Begin();
					// enable depth test for the later rendering
					glDepthMask(true);
					glEnable(GL_DEPTH_TEST);
					break;
				case Camera::ClearMode::backgroundColor:
					glClearColor(
							camera.DeRef()->backgroundColor.r,
							camera.DeRef()->backgroundColor.g,
							camera.DeRef()->backgroundColor.b,
							camera.DeRef()->backgroundColor.a);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
					break;
				case Camera::ClearMode::none:
					break;
				}

				Application::Instance->renderer.SetViewMatrix(camera.DeRef()->GetViewMatrix());
				Application::Instance->renderer.SetProjectionMatrix(camera.DeRef()->GetProjectionMatrix(
						{ (float)_framebufferWidth, (float)_framebufferHeight }));
			}
			else
			{
				if (!_hasWarningBeenShown)
				{
					PLATINUM_WARNING("Scene has no camera component. Cannot render anything.");
					_hasWarningBeenShown = true;
				}
			}
			Application::Instance->scene.Render(Application::Instance->renderer);
			Application::Instance->renderer.End();

			// unbind framebuffer
			_renderTexture.Unbind();

			// reset setting after rendering
			glDisable(GL_DEPTH_TEST);

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
				{ Maths::Vec3(-1.0f, 1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, -1.0f, -1.0f), Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, -1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, 1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },

				{ Maths::Vec3(-1.0f, -1.0f, 1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, -1.0f, -1.0f), Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, 1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, 1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, 1.0f, 1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, -1.0f, 1.0f),  Maths::Vec3(), Maths::Vec2() },

				{ Maths::Vec3(1.0f, -1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, 1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, 1.0f),    Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, 1.0f),    Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, -1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },

				{ Maths::Vec3(-1.0f, -1.0f, 1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, 1.0f, 1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, 1.0f),    Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, 1.0f),    Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, 1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, -1.0f, 1.0f),  Maths::Vec3(), Maths::Vec2() },

				{ Maths::Vec3(-1.0f, 1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, -1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, 1.0f),    Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, 1.0f, 1.0f),    Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, 1.0f, 1.0f),   Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, 1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },

				{ Maths::Vec3(-1.0f, -1.0f, -1.0f), Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, -1.0f, 1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, -1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(-1.0f, -1.0f, 1.0f),  Maths::Vec3(), Maths::Vec2() },
				{ Maths::Vec3(1.0f, -1.0f, 1.0f),   Maths::Vec3(), Maths::Vec2() },
		};

		std::vector<GLuint> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
										16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
										30, 31, 32, 33, 34, 35 };


		_skyBoxShaderInput.Set(skyboxVertices, indices);
	}

}