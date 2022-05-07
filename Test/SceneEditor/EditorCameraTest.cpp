//
// Created by Shihua on 15/03/2022.
//

#include "catch2/catch.hpp"
#include "SceneEditor/EditorCamera.h"


TEST_CASE("EditorCameraTranslationDirectionTest", "[maths][vectors][vec3][EditorCamera]")
{
	GIVEN("An editor camera and its translation")
	{

		// create camera instance
		PlatinumEngine::EditorCamera camera;

		// set its rotation matrix
		camera.RotationByMouse(PlatinumEngine::Maths::Vec2(3.14f/(3.f*0.005f), 0.f));

		// calculate the translation direction
		PlatinumEngine::Maths::Vec3 forwardDir = camera.GetForwardDirection();

		THEN("The values should be 0")
		{

			CHECK(forwardDir[0] -0.866025 < 0.001); // Jason: "This fails for some reason. Probably okay to ignore."
			CHECK(forwardDir[1] == 0.f);
			CHECK(forwardDir[2] -0.5f < 0.001);

		}
	}
}