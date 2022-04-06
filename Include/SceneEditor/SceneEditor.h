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
#include <ComponentComposition/RenderComponent.h>

// Input manager
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
			ImVec2 mouseClickedPosition = _inputManager->GetMousePosition();

			if(GameObject* result = DoRayCasting(mouseClickedPosition);
			result != nullptr)
				std::cout<< result->name << std::endl;
		}

		GameObject* DoRayCasting(ImVec2& clickedPosition)
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
				float nearPanelCoordinateForClickedPosition_x =(clickedPosition.x *2.f / (float)_framebufferWidth)- 1.f;
				float nearPanelCoordinateForClickedPosition_y =(clickedPosition.y * 2.f / (float)_framebufferHeight)- 1.f;

				// Clipping space
				PlatinumEngine::Maths::Vec4 worldCoordinateForClickedPositionCandidate4D(nearPanelCoordinateForClickedPosition_x,
						nearPanelCoordinateForClickedPosition_y, 1.0f, 1.0f);

				// View space
				worldCoordinateForClickedPositionCandidate4D = PlatinumEngine::Maths::Inverse(_camera.projectionMatrix4) * worldCoordinateForClickedPositionCandidate4D;

				// World space
				worldCoordinateForClickedPositionCandidate4D = PlatinumEngine::Maths::Inverse(_camera.viewMatrix4) * worldCoordinateForClickedPositionCandidate4D;

				PlatinumEngine::Maths::Vec3 worldCoordinateForClickedPosition(
						worldCoordinateForClickedPositionCandidate4D.x/worldCoordinateForClickedPositionCandidate4D.w,
						worldCoordinateForClickedPositionCandidate4D.y/worldCoordinateForClickedPositionCandidate4D.w,
						worldCoordinateForClickedPositionCandidate4D.z/worldCoordinateForClickedPositionCandidate4D.w);


				// Calculate ray
				ray = worldCoordinateForClickedPosition - _camera.GetCameraPosition();
			}


			//----------------------------------------//
			// Loop through Every root game object    //
			//----------------------------------------//

			// variable that store the current selected game object
			GameObject* currentSelectedGameobject = nullptr;
			float closetZValue = (float)_far;

			// loop through all the root game object from scene class
			unsigned int numberOfRootGameobject = _scene->GetRootGameObjectsCount();

			for(int gameObjectIndex =0; gameObjectIndex < numberOfRootGameobject; gameObjectIndex++)
			{
				// get current game object
				GameObject* currentGameobject = _scene->GetRootGameObject(gameObjectIndex);

				// check if the object enable
				if(currentGameobject->IsEnabledInHierarchy())
				{
					// get result for whether this game object or one of its children is selected
					currentSelectedGameobject = CheckIntersectionWithTrianglePrimitiveForGameobject(currentGameobject, ray,
							_camera.GetCameraPosition(), currentSelectedGameobject, closetZValue);

				}
			}
			return currentSelectedGameobject;
		}

		// for trianlge mesh only
		GameObject* CheckIntersectionWithTrianglePrimitiveForGameobject(GameObject* currentCheckingGameobject, PlatinumEngine::Maths::Vec3 inRay,
				const PlatinumEngine::Maths::Vec3& inCameraPosition, GameObject* currentSelectedGameObject, float& closestZValueForCrossPoint)
		{

			// check if the game object enable
			if(!currentCheckingGameobject->IsEnabledInHierarchy())
				return currentSelectedGameObject;

			//------------------//
			// Do intersection  //
			//------------------//

			// check if the game object has mesh component
			if( auto renderComponent = currentCheckingGameobject->GetComponent<RenderComponent>(); renderComponent!= nullptr)
			{
				// fetch mesh
				Mesh mesh = renderComponent->GetMesh();

				// loop all the vertices
				for(int count =0; count < mesh.indices.size(); count+=3)
				{
					//----------------------------------//
					// Do ray and triangle intersection //
					//----------------------------------//

					// build the PCS using the triangle primitive
					Maths::Vec3 originForPCS = mesh.vertices[mesh.indices[count + 0]].position;

					Maths::Vec3 axisUForPCS = (mesh.vertices[mesh.indices[count + 1]].position - originForPCS);
					float lengthU = sqrtf(LengthSquared(axisUForPCS));
					axisUForPCS = axisUForPCS/lengthU;

					Maths::Vec3 axisNForPCS = Cross(axisUForPCS, (mesh.vertices[mesh.indices[count + 2]].position - originForPCS));
					float lengthN = sqrtf(LengthSquared(axisNForPCS));
					axisNForPCS = axisNForPCS/lengthN;

					Maths::Vec3 axisWForPCS = Cross(axisUForPCS, axisNForPCS);
					float lengthW = sqrtf(LengthSquared(axisWForPCS));
					axisWForPCS = axisWForPCS/lengthW;

					// find the intersection point for ray and the panel that contains the triangle
					Maths::Vec3 vectorBetweenOriginForPCSandCameraPosition = originForPCS - inCameraPosition;

					float rateForLineApproachingPanel = Maths::Dot(vectorBetweenOriginForPCSandCameraPosition, axisNForPCS) / Maths::Dot(inRay, axisNForPCS);

					Maths::Vec3 crossPoint = inCameraPosition + inRay * rateForLineApproachingPanel;

					// Convert the cross point and the vertices of the triangle into PCS coordinate
					Maths::Mat3 PCSMatrix3x3(std::array<float, 9>({
							axisUForPCS.x, axisWForPCS.x, axisNForPCS.x,
							axisUForPCS.y, axisWForPCS.y, axisNForPCS.y,
							axisUForPCS.z, axisWForPCS.z, axisNForPCS.z
							}).data());
					// 1. cross point
					Maths::Vec3 crossPointPCS = PCSMatrix3x3 * (crossPoint - originForPCS);

					// 2. triangle vertices
					Maths::Vec3 vertex0 = PCSMatrix3x3 * (mesh.vertices[mesh.indices[count + 0]].position - originForPCS);
					Maths::Vec3 vertex1 = PCSMatrix3x3 * (mesh.vertices[mesh.indices[count + 1]].position - originForPCS);
					Maths::Vec3 vertex2 = PCSMatrix3x3 * (mesh.vertices[mesh.indices[count + 2]].position - originForPCS);

					// Do barycentric interpolation to check
					Maths::Vec3 xValueForBarycentricInterpolation((vertex0 - vertex1).x, (vertex0 - vertex2).x, (crossPointPCS - vertex0).x);
					xValueForBarycentricInterpolation = xValueForBarycentricInterpolation / sqrtf(LengthSquared(xValueForBarycentricInterpolation));

					Maths::Vec3 yValueForBarycentricInterpolation((vertex0 - vertex1).y, (vertex0 - vertex2).y, (crossPointPCS - vertex0).y);
					yValueForBarycentricInterpolation = yValueForBarycentricInterpolation/ sqrtf(LengthSquared(yValueForBarycentricInterpolation));

					// Use the features of cross product to calculate the coefficients for interpolation
					Maths::Vec3 coefficientForInterpolation = Maths::Cross(xValueForBarycentricInterpolation , yValueForBarycentricInterpolation);

					// make sure the z is not zero
					if(coefficientForInterpolation.z != 0)
					{
						// check if the final coefficients for interpolation are all positive
						if (1.f - ((coefficientForInterpolation.x + coefficientForInterpolation.y) /
								 coefficientForInterpolation.z) > 0.f
							&& coefficientForInterpolation.x / coefficientForInterpolation.z > 0.f
							&& coefficientForInterpolation.y / coefficientForInterpolation.z > 0.f)
						{
							// compare z value for the current cross point and the current farthest z value from previous cross point

							//1. if the clipping space pointing with the same direction of left-hand coordinate
							if (_far - _near > 0 && closestZValueForCrossPoint > crossPoint.z)
							{
								// update the selected game object and the closet z value if the current checking object is the closet
								currentSelectedGameObject = currentCheckingGameobject;
								closestZValueForCrossPoint = crossPointPCS.z;
							}
							//2. if the clipping space pointing with the same direction of right-hand coordinate
							else if (_far - _near < 0 && closestZValueForCrossPoint < crossPoint.z)
							{
								// update the selected game object and the closet z value if the current checking object is the closet
								currentSelectedGameObject = currentCheckingGameobject;
								closestZValueForCrossPoint = crossPointPCS.z;
							}

						}
					}
				}
			}

			// Keep checking the children of currentCheckingGameobject
			// if children exist, call this function for every child
			for(int counter = 0; counter < currentCheckingGameobject->GetChildrenCount()>0; counter++)
			{
				currentSelectedGameObject = CheckIntersectionWithTrianglePrimitiveForGameobject(currentCheckingGameobject->GetChild(counter), inRay,
						inCameraPosition, currentSelectedGameObject, closestZValueForCrossPoint);
			}

			return currentSelectedGameObject;
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

		// Values for input control
		ImVec2 _mouseMoveDelta;
		int _mouseButtonType;
		float _wheelValueDelta;

		// Value for ray casting
		GameObject* selectedGameobject;

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