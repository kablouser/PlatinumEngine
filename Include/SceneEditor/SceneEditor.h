//
// Created by Shihua on 02/03/2022.
//

#pragma once

// Platinum Library

// Scene editor
#include <SceneEditor/EditorCamera.h>

// Renderer
#include <Renderer/Renderer.h>
#include <OpenGL/Framebuffer.h>

// Game object related
#include <SceneManager/Scene.h>


// Input manager
#include <InputManager/InputManager.h>
#include <IconsKenney.h>
namespace PlatinumEngine
{
	class SceneEditor
	{
	public:
		// ___VARIABLE___
		// Value for ray casting
		GameObject* selectedGameobject;

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

		/**
		 * Function to detect which object shown in scene editor is selected
		 * @param outSelectedGameobject : the selected game object pointer, which is for output the final selected result
		 */
		void SelectGameObjectFromScene();

		/**
		 * Doing ray casting to find the object that is clicked by mouse
		 * @param clickedPosition : the mouse position when it is clicked
		 * @return
		 */
		GameObject* DoRayCasting(ImVec2& clickedPosition);


		/**
		 * checking intersection between ray and triangles
		 * @param currentCheckingGameobject : the game object which we are doing ray-triangle intersection check on.
		 * @param inRay : the ray input.
		 * @param inCameraPosition : the camera position input.
		 * @param currentSelectedGameObject : the game object that which must be intersect with ray and have the shortest
		 * 									distance to the near panel before doing the checking on the current checking game object.
		 * @param closestZValueForCrossPoint  : the z value which is closest to the z value for the near panel.
		 * @return : return the selected game object with the shortest distance to the near panel.
		 */
		GameObject* CheckRayTriangleIntersectionForGameobject(GameObject* currentCheckingGameobject, PlatinumEngine::Maths::Vec3 inRay,
				const PlatinumEngine::Maths::Vec3& inCameraPosition, GameObject* currentSelectedGameObject, float& closestZValueForCrossPoint);

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

		// Values for input control
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