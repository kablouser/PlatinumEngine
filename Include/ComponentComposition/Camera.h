#pragma once

#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "ComponentComposition/Component.h"
#include "ComponentComposition/Color.h"
#include "ComponentComposition/GameObject.h"
#include "ComponentComposition/Transform.h"

namespace PlatinumEngine
{
	class Camera: public Component
	{
	public:

		enum class ProjectionType {perspective, orthographic};
		enum class ClearMode {none, backgroundColor, skybox };

		ProjectionType projectionType;
		ClearMode clearMode;
		Color backgroundColor;
		float fov;
		float nearClippingPlane;
		float farClippingPlane;
		float orthographicSize;

		Camera();

		Maths::Vec3 ViewportToWorldPoint(
				const Maths::Vec3& viewportCoordinate,
				const Maths::Vec2& viewportSize);
		Maths::Vec3 WorldToViewportPoint(
				const Maths::Vec3& worldCoordinate,
				const Maths::Vec2& viewportSize);

		Maths::Mat4 GetWorldToClipMatrix(const Maths::Vec2& viewportSize);
		Maths::Mat4 GetClipToWorldMatrix(const Maths::Vec2& viewportSize);
		Maths::Mat4 GetProjectionMatrix(const Maths::Vec2& viewportSize);
		//Also known as World To Camera Matrix
		Maths::Mat4 GetViewMatrix();
		//Also known as View Matrix
		Maths::Mat4 GetWorldToCameraMatrix();
		//Also known as Inverse View Matrix
		Maths::Mat4 GetCameraToWorldMatrix();
	};
}