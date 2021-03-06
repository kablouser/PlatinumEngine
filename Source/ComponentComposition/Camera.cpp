#include "ComponentComposition/Camera.h"

namespace PlatinumEngine
{
	void Camera::CreateTypeInfo(TypeDatabase& database)
	{
//		ProjectionType projectionType;
//		ClearMode clearMode;
//		Color backgroundColor;
//		float fov;
//		float nearClippingPlane;
//		float farClippingPlane;
//		float orthographicSize;
		database.BeginTypeInfo<Camera>()
				.WithInherit<Component>()
				// enum types are created automatically in the database
				.WithField<ProjectionType>("projectionType", PLATINUM_OFFSETOF(Camera, projectionType))
				.WithField<ClearMode>("clearMode", PLATINUM_OFFSETOF(Camera, clearMode))

				.WithField<Color>("backgroundColor", PLATINUM_OFFSETOF(Camera, backgroundColor))
				.WithField<float>("fov", PLATINUM_OFFSETOF(Camera, fov))
				.WithField<float>("nearClippingPlane", PLATINUM_OFFSETOF(Camera, nearClippingPlane))
				.WithField<float>("farClippingPlane", PLATINUM_OFFSETOF(Camera, farClippingPlane))
				.WithField<float>("orthographicSize", PLATINUM_OFFSETOF(Camera, orthographicSize));
	}

	Camera::Camera()
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
	Maths::Vec3 Camera::ViewportToWorldPoint(
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
	Maths::Vec3 Camera::WorldToViewportPoint(
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
	Maths::Mat4 Camera::GetWorldToClipMatrix(const Maths::Vec2& viewportSize)
	{
		return (GetProjectionMatrix(viewportSize) * GetWorldToCameraMatrix());
	}

	//Returns matrix that convert from clip space to world space (Inverse of View*Projection matrix)
	Maths::Mat4 Camera::GetClipToWorldMatrix(const Maths::Vec2& viewportSize)
	{
		return Maths::Inverse(GetWorldToClipMatrix(viewportSize));
	}

	//Returns the Projection matrix
	Maths::Mat4 Camera::GetProjectionMatrix(const Maths::Vec2& viewportSize)
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

	Maths::Mat4 Camera::GetViewMatrix()
	{
		return GetWorldToCameraMatrix();
	}

	//Returns matrix that convert from world coordinates to camera coordinates (View matrix)
	Maths::Mat4 Camera::GetWorldToCameraMatrix()
	{
		Maths::Mat4 worldToCameraMatrix;
		SavedReference<Transform> tc = GetComponent<Transform>();
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

	Maths::Mat4 Camera::GetViewMatrixRotationOnly()
	{
		Maths::Mat4 worldToCameraMatrix;
		SavedReference<Transform> tc = GetGameObject().DeRef()->GetComponent<Transform>();
		if (tc.DeRef() != nullptr)
		{
			Maths::Mat4 invR, invT;
			invR = Maths::Quaternion::QuaternionToMatrix(Maths::Quaternion::Inverse(tc.DeRef()->localRotation));
			worldToCameraMatrix = invR;
		}
		else
			worldToCameraMatrix.SetIdentityMatrix();
		return worldToCameraMatrix;
	}

	//Returns matrix that convert from camera coordinates to world coordinates (Inverse View matrix)
	Maths::Mat4 Camera::GetCameraToWorldMatrix()
	{
		Maths::Mat4 cameraToWorldMatrix;
		SavedReference<Transform> tc = GetComponent<Transform>();
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