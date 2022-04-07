#include "ComponentComposition/CameraComponent.h"

namespace PlatinumEngine
{
    CameraComponent::CameraComponent()
    {
         projection = projectionType::PERSPECTIVE;
		 backgroundColor = Color(0,0,0,255);
		 fov = 60;
		 nearClippingPlane = 0.3f;
		 farClippingPlane = 1000;
		 size = 1;
		 aspectRatio = 1;
		 viewportSize = Maths::Vec2(1.f,1.f);
		 viewportPos = Maths::Vec2(0.f,0.f);
		 clearFlag = clearFlagType::NONE;
    }
	CameraComponent::~CameraComponent()
	{}

	//Copy values from another camera
	void CameraComponent::CopyFrom(CameraComponent other)
	{
		projection = other.projection;
		backgroundColor = other.backgroundColor;
		fov = other.fov;
		nearClippingPlane = other.nearClippingPlane;
		farClippingPlane = other.farClippingPlane;
		aspectRatio = other.aspectRatio;
		size = other.size;
		viewportSize = other.viewportSize;
		viewportPos = other.viewportPos;
		clearFlag = other.clearFlag;
	}

	Maths::Mat4 CameraComponent::CalculateObliqueMatrix(Maths::Vec4 clipPlane)
	{
		Maths::Mat4 projectionMatrix = GetProjectionMatrix();
		Maths::Vec4 q = Maths::Inverse(projectionMatrix) * Maths::Vec4(sgn(clipPlane.x), sgn(clipPlane.y), 1.0f, 1.0f);
		Maths::Vec4 c = clipPlane * (2.0F / (Maths::Dot (clipPlane, q)));
		// third row = clip plane - fourth row
		projectionMatrix[2][0] = c.x - projectionMatrix[3][0];
		projectionMatrix[2][1] = c.y - projectionMatrix[3][1];
		projectionMatrix[2][2] = c.z - projectionMatrix[3][2];
		projectionMatrix[2][3] = c.w - projectionMatrix[3][3];
		return projectionMatrix;
	}

	void CameraComponent::Render()
	{

	}

	void CameraComponent::ResetProjectionMatrix()
	{
		if (viewportSize.y != 0)
			aspectRatio = (viewportSize.x / viewportSize.y);
		else
			aspectRatio = 1.0f;
	}

	//Transform point from screen to viewport coordinates
	Maths::Vec3 CameraComponent::ScreenToViewportPoint(Maths::Vec3 position)
	{
		float nx = (position.x - viewportPos.x) / viewportSize.x;
		float ny = (position.y - viewportPos.y) / viewportSize.y;
		return Maths::Vec3(nx, ny, position.z);
	}
	//Transform point from screen to world coordinates
	Maths::Vec3 CameraComponent::ScreenToWorldPoint(Maths::Vec3 position)
	{
		Maths::Vec3 o;
		Maths::Mat4 clipToWorld = GetClipToWorldMatrix();
		if( !CameraUnProject( position, GetCameraToWorldMatrix(), clipToWorld, viewportSize,o))
		{
			//AssertString (Format("Screen position out of view frustum (screen pos %f, %f, %f) (Camera rect %d %d %d %d)", v.x, v.y, v.z, viewport.x, viewport.y, viewport.width, viewport.height));
		}
		return o;
	}
	//Transform point from viewport to screen coordinates
	Maths::Vec3 CameraComponent::ViewportToScreenPoint(Maths::Vec3 position)
	{
		float nx = position.x * viewportSize.x + viewportPos.x;
		float ny = position.y * viewportSize.y + viewportPos.y;
		return Maths::Vec3(nx, ny, position.z);
	}
	//Transform point from viewport to world coordinates
	Maths::Vec3 CameraComponent::ViewportToWorldPoint(Maths::Vec3 position)
	{
		Maths::Vec3 screenPoint = ViewportToScreenPoint (position);
		return ScreenToWorldPoint (screenPoint);
	}
	//Transform point from world to screen coordinates
	Maths::Vec3 CameraComponent::WorldToScreenPoint(Maths::Vec3 position)
	{
		Maths::Vec3 o;
		Maths::Mat4 clipToWorld = GetClipToWorldMatrix();
		CameraProject( position, GetCameraToWorldMatrix(), clipToWorld, viewportSize, o);
		return o;
	}
	//Transform point from world to viewport coordinates
	Maths::Vec3 CameraComponent::WorldToViewportPoint(Maths::Vec3 position)
	{
		Maths::Vec3 screenPoint = WorldToScreenPoint (position);
		return ScreenToViewportPoint (screenPoint);
	}
	//Returns matrix that convert from world space to clip space
	Maths::Mat4 CameraComponent::GetWorldToClipMatrix()
	{
		return (GetProjectionMatrix() * GetWorldToCameraMatrix());
	}
	//Returns matrix that convert from clip space to world space
	Maths::Mat4 CameraComponent::GetClipToWorldMatrix()
	{
		return Maths::Inverse(GetWorldToClipMatrix());
	}
	//Returns the projection matrix
	Maths::Mat4 CameraComponent::GetProjectionMatrix()
	{
		float m_Aspect = (float)viewportSize.x / viewportSize.y;
		Maths::Mat4 projectionMatrix;
		if (projection==projectionType::PERSPECTIVE)
			projectionMatrix.SetPerspectiveMatrix( fov, aspectRatio, nearClippingPlane, farClippingPlane );
		else
			projectionMatrix.SetOrthogonalMatrix( -size * m_Aspect, size * m_Aspect, -size, size, nearClippingPlane, farClippingPlane );
		return projectionMatrix;
	}
	//Returns matrix that convert from world coordinates to camera coordinates
	Maths::Mat4 CameraComponent::GetWorldToCameraMatrix()
	{
		Maths::Mat4 worldToCameraMatrix;
		worldToCameraMatrix.SetScaleMatrix(Maths::Vec3(1.0F, 1.0F, -1.0F));
		TransformComponent* tc = GetGameObject()->GetComponent<TransformComponent>();
		worldToCameraMatrix = worldToCameraMatrix * tc->GetWorldToLocalMatrixNoScale();
		return worldToCameraMatrix;
	}
	//Returns matrix that convert from camera coordinates to world coordinates
	Maths::Mat4 CameraComponent::GetCameraToWorldMatrix()
	{
		return Maths::Inverse(GetWorldToCameraMatrix());
	}
	//Projects 3D space onto the 2D screen
	bool CameraComponent::CameraProject(Maths::Vec3 p, Maths::Mat4 cameraToWorld, Maths::Mat4 worldToClip,
			Maths::Vec2 viewport, Maths::Vec3& outP)
	{
		Maths::Vec3 clipPoint;
		outP = Maths::Vec3(0, 0, 0);
		if(PerspectiveMultiplyPoint3(worldToClip, p, clipPoint))
		{
			Maths::Vec3 cameraPos = GetPosition(cameraToWorld);
			Maths::Vec3 dir = p - cameraPos;
			// The camera/projection matrices follow OpenGL convention: positive Z is towards the viewer.
			// So negate it to get into Unity convention.
			Maths::Vec3 forward = -GetAxisZ(cameraToWorld);
			float dist = Maths::Dot( dir, forward );

			outP.x = viewport.x + (1.0f + clipPoint.x) * viewport.x * 0.5f;
			outP.y = viewport.y + (1.0f + clipPoint.y) * viewport.y * 0.5f;
			//outP.z = (1.0f + clipPoint.z) * 0.5f;
			outP.z = dist;

			return true;
		}
		outP = Maths::Vec3( 0.0f, 0.0f, 0.0f );
		return false;
	}
	//UnProjects 3D space from the 2D screen
	bool CameraComponent::CameraUnProject(Maths::Vec3 p, Maths::Mat4 cameraToWorld, Maths::Mat4 clipToWorld,
			Maths::Vec2 viewport, Maths::Vec3& outP)
	{
		// pixels to -1..1
		Maths::Vec3 in_v;
		in_v.x = (p.x - viewport.x) * 2.0f / viewport.x - 1.0f;
		in_v.y = (p.y - viewport.y) * 2.0f / viewport.y - 1.0f;
		// It does not matter where the point we unproject lies in depth; so we choose 0.95, which
		// is further than near plane and closer than far plane, for precision reasons.
		// In a perspective camera setup (near=0.1, far=1000), a point at 0.95 projected depth is about
		// 5 units from the camera.
		in_v.z = 0.95f;

		Maths::Vec3 pointOnPlane;
		if(PerspectiveMultiplyPoint3(clipToWorld, in_v, pointOnPlane))
		{
			// Now we have a point on the plane perpendicular to the viewing direction. We need to return the one that is on the line
			// towards this point, and at p.z distance along camera's viewing axis.
			Maths::Vec3 cameraPos = GetPosition(cameraToWorld);
			Maths::Vec3 dir = pointOnPlane - cameraPos;

			// The camera/projection matrices follow OpenGL convention: positive Z is towards the viewer.
			// So negate it to get into Unity convention.
			Maths::Vec3 forward = -GetAxisZ(cameraToWorld);
			float distToPlane = Maths::Dot( dir, forward );
			if( abs(distToPlane) >= 1.0e-6f )
			{
				if(IsPerspective(clipToWorld))
				{
					dir *= p.z / distToPlane;
					outP = cameraPos + dir;
				}
				else
				{
					outP = pointOnPlane - forward * (distToPlane - p.z);
				}
				return true;
			}
		}
		outP = Maths::Vec3( 0.0f, 0.0f, 0.0f );
		return false;
	}

	float CameraComponent::sgn(float n)
	{
		if (n > 0.0f) return 1.0f;
		if (n < 0.0f) return -1.0f;
		return 0.0f;
	}

	bool CameraComponent::PerspectiveMultiplyPoint3(Maths::Mat4 m, Maths::Vec3 v, Maths::Vec3 o)
	{
		Maths::Vec3 res;
		float w;
		res.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
		res.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
		res.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
		w     = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];
		if( abs(w) > 1.0e-7f )
		{
			float invW = 1.0f / w;
			o.x = res.x * invW;
			o.y = res.y * invW;
			o.z = res.z * invW;
			return true;
		}
		return false;
	}
	//Z axis (forward vector) of camera
	Maths::Vec3 CameraComponent::GetAxisZ(Maths::Mat4 m)
	{
		return Maths::Vec3(m[0][2], m[1][2], m[2][2]);
	}
	//Camera position
	Maths::Vec3 CameraComponent::GetPosition(Maths::Mat4 m)
	{
		return Maths::Vec3(m[0][3], m[1][3], m[2][3]);
	}

	bool CameraComponent::IsPerspective(Maths::Mat4 m)
	{
		return (m[0][3] != 0.0f || m[1][3] != 0.0f || m[2][3] != 0.0f || m[3][3] != 1.0f);
	}
}