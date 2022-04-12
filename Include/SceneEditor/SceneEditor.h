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

		/**
		 * Update selected game object
		 * @param inGameObject
		 */
		void SetSelectedGameobject(GameObject* inGameObject);

		/**
		 * Get the private selected game object
		 * @return
		 */
		GameObject* GetSelectedGameobject();


		// ___CONSTRUCTOR___
		SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer);


		/**
		 * This is a helper function used for creating the gameobject gizmo
		 * and the viewGizmo on the top right of the scene editor
		 * @param cameraView
		 * @param cameraProjection
		 * @param matrix
		 * @param editTransformDecomposition
		 */
		void UseGizmo(float* cameraView, float* cameraProjection, ImGuizmo::MODE);
	private:

		// ___PARAMETERS___
		// Value for ray casting
		GameObject* _selectedGameobject;
		
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

		// Values for input control
		ImVec2 _mouseMoveDelta;
		int _mouseButtonType;
		float _wheelValueDelta;

		// output of OpenGL rendering
		Framebuffer _renderTexture;
		int _framebufferWidth;
		int _framebufferHeight;

	};
}