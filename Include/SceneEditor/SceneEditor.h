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
#include <IconsFontAwesome6.h>
#include <ImGuizmo.h>

//Asset Helper
#include "AssetDatabase/AssetHelper.h"

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
		void Update(ImVec2 targetSize, bool IsProjectionUpdated, ImGuizmo::MODE, ImGuizmo::OPERATION);

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
		GameObject* FindClickedObject(ImVec2& clickedPosition);


		/**
		 * Using ray casting to find out if the current game object or its children is selected
		 * @param currentCheckingGameobject : the game object which is going to be checked.
		 * @param inRay : the ray input.
		 * @param inCameraPosition : the camera position input.
		 * @param currentSelectedGameObject : the game object that is confirmed as the selected game object temporarily.
		 * @param closestZValueForCrossPoint  : the z value of the intersection/clicked point of the previous temporary selected game object.
		 * @return : return the selected game object with the shortest distance to the near panel.
		 */
		GameObject* UpdateSelectedGameObject(GameObject* currentCheckingGameobject, PlatinumEngine::Maths::Vec3 inRay,
				const PlatinumEngine::Maths::Vec3& inCameraPosition, GameObject* currentSelectedGameObject, float& closestZValueForCrossPoint);

		/**
		 * Transforming world coordinate into screen coordinate and use interpolation
		 * to find out if the current game object or its children is selected
		 * @param currentCheckingGameobject : the game object which is going to be checked.
		 * @param currentSelectedGameObject :the game object that is confirmed as the selected game object temporarily.
		 * @param clickedPosition : the screen coordinate of the clicked position.
		 * @param closestZValueForCrossPoint : the z value of the intersection/clicked point of the previous temporary selected game object.
		 * @return : return the selected game object with the shortest distance to the near panel.
		 */
		GameObject* UpdateSelectedGameObject(GameObject* currentCheckingGameobject, GameObject* currentSelectedGameObject, Maths::Vec2 clickedPosition, float& closestZValueForCrossPoint);


		/**
		 * Convert Screen Coordinate To World Coordinate
		 * @param coordinate : 2d screen coordinate
		 * @return
		 */
		Maths::Vec3 ConvertScreenCoordinateToWorldCoordinate(const Maths::Vec2& coordinate);

		/**
		 * Convert World Coordinate To Screen Coordinate
		 * @param coordinate : 3d world coordinate
		 * @return
		 */
		Maths::Vec3 ConvertWorldCoordinateToScreenCoordinate(const Maths::Vec3& coordinate);

		/**
		 * Check if the point is an internal point of the triangle and calculate the interpolation values for it
		 * @param triangle : array of triangle vertices
		 * @param point    : the point that we want to check
		 * @param x        : the coefficient for the first triangle vertex in the vec3 array
		 * @param y		   : the coefficient for the second triangle vertex in the vec3 array
		 * @param z 	   : the coefficient for the third triangle vertex in the vec3 array
		 * @return		   : the boolean that claim if the point is inside the triangle (claim if the interpolation success)
		 */
		bool DoInterpolationCheck(std::array<Maths::Vec2,3> triangle, Maths::Vec2 point, float& x, float& y, float& z);

		/**
		 * Update selected game object
		 * @param inGameObject
		 */
		void SetSelectedGameobject(GameObject* inGameObject);

		void DeleteSelectedGameObject();

		/**
		 * Get the private selected game object
		 * @return
		 */
		GameObject* GetSelectedGameobject();



		/**
		 * This is a helper function used for creating the gameobject gizmo
		 * and the viewGizmo on the top right of the scene editor
		 * @param cameraView
		 * @param cameraProjection
		 * @param matrix
		 * @param editTransformDecomposition
		 */
		void UseGizmo(float* cameraView, float* cameraProjection, ImGuizmo::MODE, ImGuizmo::OPERATION, bool onGizmo);

		/**
		 * Function that initiate shader input for skybox
		 */
		void CreateSkyBoxShaderInput();

		/**
		 * Function that initiate shader input for grid
		 */
		void CreateGridShaderInput();

		// ___CONSTRUCTOR___
		SceneEditor(InputManager* inputManager, Scene* scene, Renderer* renderer, AssetHelper* assetHelper);

	private:

		// ___PARAMETERS___
		// Grid
		ShaderInput _gridShaderInput;

		// Skybox
		Texture _skyboxTexture;
		ShaderInput _skyBoxShaderInput;

		// Store
		GameObject* _selectedGameobject;

		//ImGuizmo
		float _snap[3];
		float _bounds[6];
		float _boundsSnap[3];

		bool _useSnap;
		bool _boundSizing;
		bool _onObjectGizmo;

		bool _boundSizingSnap;
		ImGuizmo::MODE _currentGizmoMode;
		ImGuizmo::OPERATION _currentGizmoOperation;
		// Flags
		bool _ifCameraSettingWindowOpen;

		// Managers
		InputManager* _inputManager;
		Scene* _scene;
		Renderer* _renderer;
		AssetHelper* _assetHelper;

		// Values for Camera
		EditorCamera _camera;
		int   _fov;
		float _near;
		float _far;

		// Values for input control
		ImVec2 _mouseMoveDelta;
		int _mouseButtonType;
		float _wheelValueDelta;

		// output of OpenGL rendering
		Framebuffer _renderTexture;
		int _framebufferWidth;
		int _framebufferHeight;

		// Grid transparency parameters
		float _transparency;
		bool _xGrid = false, _yGrid = true, _zGrid = false;
		bool _enableSkyBox, _enableGrid;
	};
}