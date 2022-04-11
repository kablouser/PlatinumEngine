#ifndef PLATINUMENGINE_CAMERACOMPONENT_H
#define PLATINUMENGINE_CAMERACOMPONENT_H

#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "ComponentComposition/Component.h"
#include "ComponentComposition/Color.h"
#include "ComponentComposition/GameObject.h"
#include "ComponentComposition/TransformComponent.h"

namespace PlatinumEngine
{
	enum class ProjectionType {perspective, orthographic};
	enum class ClearFlagType {skybox, bgcolor, depth, none};
	class CameraComponent: Component
	{
	public:
		ProjectionType projection;
		Color backgroundColor;
		float fov;
		float nearClippingPlane;
		float farClippingPlane;
		float othographicSize;
		Maths::Vec2 viewportPos;
		Maths::Vec2 viewportSize;
		ClearFlagType clearFlag;

	public:
		CameraComponent();
		~CameraComponent();

		Maths::Vec3 ScreenToWorldPoint(Maths::Vec3 position);
		Maths::Vec3 WorldToScreenPoint(Maths::Vec3 position);

		Maths::Mat4 GetWorldToClipMatrix();
		Maths::Mat4 GetClipToWorldMatrix();
		Maths::Mat4 GetProjectionMatrix();
		Maths::Mat4 GetWorldToCameraMatrix();
		Maths::Mat4 GetCameraToWorldMatrix();

		Maths::Vec3 CameraProject(Maths::Vec3 obj, Maths::Mat4 modelview, Maths::Mat4 proj, Maths::Vec4 viewport);
		Maths::Vec3 CameraUnProject(Maths::Vec3 win, Maths::Mat4 modelview, Maths::Mat4 proj, Maths::Vec4 viewport);


	};
}

#endif //PLATINUMENGINE_CAMERACOMPONENT_H
