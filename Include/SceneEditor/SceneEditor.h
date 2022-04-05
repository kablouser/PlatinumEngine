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
		void Update(ImVec2 targetSize);


		void SelectGameObjectFromScene()
		{
			ImVec2 mouseClickedPosition;

			DoRayCasting(mouseClickedPosition);
		}

		void DoRayCasting(ImVec2 clickedPosition)
		{
			//-----------------//
			// Calculate ray   //
			//-----------------//
			PlatinumEngine::Maths::Vec3 ray;

			// check projection type
			if(_camera.isOrthogonal)
			{
				ray = PlatinumEngine::Maths::Vec3(0.f, 0.f, 1.f);
			}
			else
			{
				// turning screen coordinate 2d back to the near panel (of the frustum) coordinate 2d
				float nearPanelCoordinate_x = (clickedPosition.x * 2 / _framebufferWidth) - 1;
				float nearPanelCoordinate_y = (clickedPosition.y * 2 / _framebufferHeight) - 1;
				// Clipping space
				PlatinumEngine::Maths::Vec4 finalCoordiante(nearPanelCoordinate_x, nearPanelCoordinate_y, _camera.nearPanel, 1.0f);

				// View space
				finalCoordiante = PlatinumEngine::Maths::Inverse(_camera.projectionMatrix4) * finalCoordiante;

				// World space
				finalCoordiante = PlatinumEngine::Maths::Inverse(_camera.viewMatrix4) * finalCoordiante;

				// Calculate ray
				finalCoordiante = finalCoordiante ; // - _camera.GetPosition();

			}


			//---------------------------------------------//
			// Loop through the primitives in an object    //
			//---------------------------------------------//

			// get all the game object from scene class
			unsigned int numberOfRootGameobject = _scene->GetRootGameObjectsCount();

			for(int gameObjectIndex; gameObjectIndex < numberOfRootGameobject; gameObjectIndex++)
			{
				GameObject* currentGameobject = _scene->GetRootGameObject(gameObjectIndex);
				if(currentGameobject->IsEnabledInHierarchy())
				{
					CheckIntersectionWithTrianglePrimitiveForGameobject(currentGameobject);
				}
			}
		}

		// for trianlge mesh only
		void CheckIntersectionWithTrianglePrimitiveForGameobject(GameObject* inGameobject)
		{
			//-----------------------------------//
			// check if they have mesh component //
			//-----------------------------------//
		}


		// ___CONSTRUCTOR___
		SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer);

		// ___DESTRUCTOR___
		//~SceneEditor();

	private:

		// ___PARAMETERS___

		// Flags
		bool _ifCameraSettingWindowOpen;

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