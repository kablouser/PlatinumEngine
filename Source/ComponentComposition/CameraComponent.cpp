#include "ComponentComposition/CameraComponent.h"

namespace PlatinumEngine
{
	void CameraComponent::CreateTypeInfo(TypeDatabase& database)
	{
//		ProjectionType projectionType;
//		ClearMode clearMode;
//		Color backgroundColor;
//		float fov;
//		float nearClippingPlane;
//		float farClippingPlane;
//		float orthographicSize;
		database.BeginTypeInfo<CameraComponent>()
				.WithInherit<Component>()
				// enum types are created automatically in the database
				.WithField<ProjectionType>("projectionType", PLATINUM_OFFSETOF(CameraComponent, projectionType))
				.WithField<ClearMode>("clearMode", PLATINUM_OFFSETOF(CameraComponent, clearMode))

				.WithField<Color>("backgroundColor", PLATINUM_OFFSETOF(CameraComponent, backgroundColor))
				.WithField<float>("fov", PLATINUM_OFFSETOF(CameraComponent, fov))
				.WithField<float>("nearClippingPlane", PLATINUM_OFFSETOF(CameraComponent, nearClippingPlane))
				.WithField<float>("farClippingPlane", PLATINUM_OFFSETOF(CameraComponent, farClippingPlane))
				.WithField<float>("orthographicSize", PLATINUM_OFFSETOF(CameraComponent, orthographicSize));
	}

	CameraComponent::CameraComponent()
	{
		projectionType = ProjectionType::perspective;
		backgroundColor = Color(0.f, 0.f, 0.f, 1.f);
		fov = 60;
		nearClippingPlane = 0.3f;
		farClippingPlane = 1000;
		orthographicSize = 1;
		clearMode = ClearMode::backgroundColor;
	}

	//Transform point from screen to world coordinates
	Maths::Vec3 CameraComponent::ViewportToWorldPoint(
			const Maths::Vec3& viewportCoordinate,
			const Maths::Vec2& viewportSize)
	{
		auto view = GetViewMatrix();
		auto projection = GetProjectionMatrix(viewportSize);

		glm::vec3 win = viewportCoordinate;
		glm::vec4 vp(0, 0, viewportSize.x, viewportSize.y);
		glm::mat4 m = (glm::mat4)view;
		glm::mat4 p = (glm::mat4)projection;

		glm::vec3 result = glm::unProject(win, m, p, vp);
		return { result.x, result.y, result.z };
	}

	//Transform point from world to screen coordinates
	Maths::Vec3 CameraComponent::WorldToViewportPoint(
			const Maths::Vec3& worldCoordinate,
			const Maths::Vec2& viewportSize)
	{
		auto view = GetViewMatrix();
		auto projection = GetProjectionMatrix(viewportSize);

		glm::vec3 o = worldCoordinate;
		glm::vec4 vp(0, 0, viewportSize.x, viewportSize.y);
		glm::mat4 m = (glm::mat4)view;
		glm::mat4 p = (glm::mat4)projection;

		glm::vec3 result = glm::project(o, m, p, vp);
		return { result.x, result.y, result.z };
	}

	//Returns matrix that convert from world space to clip space (View*Projection matrix)
	Maths::Mat4 CameraComponent::GetWorldToClipMatrix(const Maths::Vec2& viewportSize)
	{
		return (GetProjectionMatrix(viewportSize) * GetWorldToCameraMatrix());
	}

	//Returns matrix that convert from clip space to world space (Inverse of View*Projection matrix)
	Maths::Mat4 CameraComponent::GetClipToWorldMatrix(const Maths::Vec2& viewportSize)
	{
		return Maths::Inverse(GetWorldToClipMatrix(viewportSize));
	}

	//Returns the Projection matrix
	Maths::Mat4 CameraComponent::GetProjectionMatrix(const Maths::Vec2& viewportSize)
	{
		float aspectRatio = viewportSize.x / viewportSize.y;
		Maths::Mat4 projectionMatrix;
		if (projectionType == ProjectionType::perspective)
			projectionMatrix.SetPerspectiveMatrix(fov, aspectRatio, nearClippingPlane, farClippingPlane);
		else
			projectionMatrix.SetOrthogonalMatrix(-orthographicSize * aspectRatio, orthographicSize * aspectRatio,
					-orthographicSize, orthographicSize, nearClippingPlane, farClippingPlane);
		return projectionMatrix;
	}

	Maths::Mat4 CameraComponent::GetViewMatrix()
	{
		return GetWorldToCameraMatrix();
	}

	//Returns matrix that convert from world coordinates to camera coordinates (View matrix)
	Maths::Mat4 CameraComponent::GetWorldToCameraMatrix()
	{
		Maths::Mat4 worldToCameraMatrix;
		SavedReference<TransformComponent> tc = GetComponent<TransformComponent>();
		if (tc)
		{
			Maths::Mat4 invR, invT;
			invR = Maths::Quaternion::QuaternionToMatrix(Maths::Quaternion::Inverse(tc.DeRef()->localRotation));
			invT.SetTranslationMatrix(-tc.DeRef()->localPosition);
			worldToCameraMatrix = invR * invT;
		}
		else
			worldToCameraMatrix.SetIdentityMatrix();
		return worldToCameraMatrix;
	}

	//Returns matrix that convert from camera coordinates to world coordinates (Inverse View matrix)
	Maths::Mat4 CameraComponent::GetCameraToWorldMatrix()
	{
		Maths::Mat4 cameraToWorldMatrix;
		SavedReference<TransformComponent> tc = GetComponent<TransformComponent>();
		if (tc)
		{
			Maths::Mat4 t, r;
			t.SetTranslationMatrix(tc.DeRef()->localPosition);
			r = Maths::Quaternion::QuaternionToMatrix(tc.DeRef()->localRotation);
			cameraToWorldMatrix = t * r;
		}
		else
			cameraToWorldMatrix.SetIdentityMatrix();
		return cameraToWorldMatrix;
	}
}