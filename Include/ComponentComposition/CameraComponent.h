#ifndef PLATINUMENGINE_CAMERACOMPONENT_H
#define PLATINUMENGINE_CAMERACOMPONENT_H

#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "ComponentComposition/Component.h"
#include "ComponentComposition/Color.h"

namespace PlatinumEngine
{
	enum projectionType {PERSPECTIVE, ORTHOGRAPHIC};
	enum clearFlagType {SKYBOX, BGCOLOR, DEPTH, NONE};
	class CameraComponent: Component
	{
	public:
		projectionType projection;
		Color backgroundColor;
		float fov;
		float aspectRatio;
		float nearClippingPlane;
		float farClippingPlane;
		float size;
		Maths::Vec2 viewportPos;
		Maths::Vec2 viewportSize;
		clearFlagType clearFlag;

	public:
		CameraComponent();
		~CameraComponent();
		void CopyFrom(CameraComponent other);
		Maths::Mat4 CalculateObliqueMatrix(Maths::Vec4 clipPlane);

		void Render();
		void ResetProjectionMatrix();

		Maths::Vec3 ScreenToViewportPoint(Maths::Vec3 position);
		Maths::Vec3 ScreenToWorldPoint(Maths::Vec3 position);
		Maths::Vec3 ViewportToScreenPoint(Maths::Vec3 position);
		Maths::Vec3 ViewportToWorldPoint(Maths::Vec3 position);
		Maths::Vec3 WorldToScreenPoint(Maths::Vec3 position);
		Maths::Vec3 WorldToViewportPoint(Maths::Vec3 position);

		Maths::Vec4 GetScreenViewportRect();
		Maths::Mat4 GetWorldToClipMatrix();
		Maths::Mat4 GetProjectionMatrix();
		Maths::Mat4 GetWorldToCameraMatrix();
		Maths::Mat4 GetCameraToWorldMatrix();
		Maths::Mat4 GetClipToWorldMatrix();

		bool CameraProject(Maths::Vec3 p, Maths::Mat4 cameraToWorld, Maths::Mat4 worldToClip, Maths::Vec2 viewport, Maths::Vec3& outP);
		bool CameraUnProject(Maths::Vec3 p, Maths::Mat4 cameraToWorld, Maths::Mat4 clipToWorld, Maths::Vec2 viewport, Maths::Vec3& outP);

	private:
		float sgn(float n);
	};
}

#endif //PLATINUMENGINE_CAMERACOMPONENT_H
