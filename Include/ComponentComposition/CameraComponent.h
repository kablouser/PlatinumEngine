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
		float aspectRatio;
		float nearClippingPlane;
		float farClippingPlane;
		float size;
		Maths::Vec2 viewportPos;
		Maths::Vec2 viewportSize;
		ClearFlagType clearFlag;

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

		Maths::Mat4 GetWorldToClipMatrix();
		Maths::Mat4 GetClipToWorldMatrix();
		Maths::Mat4 GetProjectionMatrix();
		Maths::Mat4 GetWorldToCameraMatrix();
		Maths::Mat4 GetCameraToWorldMatrix();

		bool CameraProject(Maths::Vec3 p, Maths::Mat4 cameraToWorld, Maths::Mat4 worldToClip, Maths::Vec2 viewport, Maths::Vec3& outP);
		bool CameraUnProject(Maths::Vec3 p, Maths::Mat4 cameraToWorld, Maths::Mat4 clipToWorld, Maths::Vec2 viewport, Maths::Vec3& outP);

	private:
		float sgn(float n);
		bool PerspectiveMultiplyPoint3(Maths::Mat4 m, Maths::Vec3 v, Maths::Vec3 o);
		Maths::Vec3 GetAxisZ(Maths::Mat4 m);
		Maths::Vec3 GetPosition(Maths::Mat4 m);
		bool IsPerspective(Maths::Mat4 m);

	};
}

#endif //PLATINUMENGINE_CAMERACOMPONENT_H
