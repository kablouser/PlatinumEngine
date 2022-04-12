//
// Created by Shihua on 02/03/2022.
//

#pragma once

// Platinum Library
#include <Renderer/Renderer.h>
#include <SceneEditor/EditorCamera.h>
#include <SceneManager/Scene.h>
#include <OpenGL/Framebuffer.h>
#include <InputManager/InputManager.h>
#include <IconsKenney.h>
#include <ImGuizmo.h>
namespace PlatinumEngine
{
	class SceneEditor
	{
	public:
		// ___VARIABLE___

		// ___FUNCTION___

		/**
		 * Show scene editor window
		 * @param outIsOpen : flag for showing this window or not
		 * @param rasterRenderer :  Reference of the engine renderer
		 */
		void ShowGUIWindow(bool* outIsOpen);

		/**
		 * Update data in Scene Editor
		 */
		void Update(ImVec2 targetSize, ImGuizmo::MODE currentGizmoMode);

		// ___CONSTRUCTOR___
		SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer);

		// ___DESTRUCTOR___
		//~SceneEditor();

		/**
		 * This is a helper function used for creating the gameobject gizmo
		 * and the viewGizmo on the top right of the scene editor
		 * @param cameraView
		 * @param cameraProjection
		 * @param matrix
		 * @param editTransformDecomposition
		 */
		void UseGizmo(float* cameraView, float* cameraProjection, ImGuizmo::MODE);

		void BuildGrid()
		{
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;

			unsigned int counter = 0;

			for(int x = -25; x<25 ;x+= 5, counter +=4)
			{
				vertices.emplace_back(
					Vertex{
				Maths::Vec3((float)x, 0.f, -25.f),
				Maths::Vec3(0.f,1.f,0.f),
				Maths::Vec2(0.f, 0.f)
					}
				);

				vertices.emplace_back(
					Vertex{
							Maths::Vec3((float)x, 0.f, 25.f),
							Maths::Vec3(0.f,0.f,0.f),
							Maths::Vec2(0.f, 0.f)
					}
				);

				vertices.emplace_back(
					Vertex{
							Maths::Vec3(-25, 0.f, (float)x),
							Maths::Vec3(0.f,0.f,0.f),
							Maths::Vec2(0.f, 0.f)
					}
				);

				vertices.emplace_back(
					Vertex{
							Maths::Vec3(25.f, 0.f, (float)x),
							Maths::Vec3(0.f,0.f,0.f),
							Maths::Vec2(0.f, 0.f)
					}
				);


				indices.emplace_back(counter);
				indices.emplace_back(counter+1);
				indices.emplace_back(counter+2);
				indices.emplace_back(counter+3);
			}

			_shaderInput.Set(vertices, indices);
		}

	private:

		// ___PARAMETERS___

		//Grid
		ShaderInput _shaderInput;

		//ImGuizmo
		float _snap[3];
		float _bounds[6];
		float _boundsSnap[3];
		float _fakeVeiwMatrix[16];

		bool _useSnap;
		bool _boundSizing;
		bool _boundSizingSnap;

		ImGuizmo::MODE _currentGizmoMode;

		// Flags
		bool _ifCameraSettingWindowOpen;
		enum ClickedZoneType{RenderingZone, GizmoZone, NoZoneClicked};
		ClickedZoneType _currentClickedZone;

		// Managers
		InputManager* _inputManager;
		Scene* _scene;
		Renderer* _renderer;

		// Values for Camera
		EditorCamera _camera;
		int _fov;
		int _near;
		int _far;

		ImVec2 _mouseMoveDelta;
		int _mouseButtonType;
		float _wheelValueDelta;


		// output of OpenGL rendering
		Framebuffer _renderTexture;
		int _framebufferWidth;
		int _framebufferHeight;

		// vertics
		std::vector<Vertex> vertices = {{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  0.0f, -1.0f  }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, {  0.0f,  0.0f, -1.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  0.0f,  1.0f  }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, {  0.0f,  0.0f,  1.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, { -1.0f,  0.0f,  0.0f }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 1.0f,  0.0f,  0.0f  }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
										{{ 0.5f, -0.5f, -0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f, -0.5f,  0.5f   }, { 0.0f, -1.0f,  0.0f  }, { 0, 0 }},
										{{ -0.5f, -0.5f,  0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f, -0.5f, -0.5f  }, {  0.0f, -1.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0, 0 }},
										{{ 0.5f,  0.5f, -0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
										{{ 0.5f,  0.5f,  0.5f   }, { 0.0f,  1.0f,  0.0f  }, { 0, 0 }},
										{{ -0.5f,  0.5f,  0.5f  }, {  0.0f,  1.0f,  0.0f }, { 0, 0 }},
										{{ -0.5f,  0.5f, -0.5f  }, {  0.0f,  1.0f,  0.0  }, { 0, 0 }}};
		std::vector<unsigned int> indices = {
				0,   1,   2,
				3,   4,   5,
				6,   7,   8,
				9,   10,  11,
				12,  13,  14,
				15,  16,  17,
				18,  19,  20,
				21,  22,  23,
				24,  25,  26,
				27,  28,  29,
				30,  31,  32,
				33,  34,  35,
				36,  37,  38,
				39,  40,  41,
				42,  43,  44,
				45,  46,  47,
				48,  49,  50,
				51,  52,  53,
				54,  55,  56,
				57,  59,  59,
				60,  61,  62,
				63,  64,  65,
				66,  67,  68,
				69,  70,  71,
				72,  73,  74,
				75,  76,  77,
				78,  79,  80,
				81,  82,  83,
				84,  85,  86,
				87,  88,  89,
				90,  91,  92,
				93,  94,  95,
				96,  97,  98,
				99,  100, 101,
				102, 103, 104,
				105, 106, 107
		};
	};
}