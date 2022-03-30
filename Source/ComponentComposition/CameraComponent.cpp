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
    }
	CameraComponent::~CameraComponent()
	{}

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
	}

	Maths::Mat4 CameraComponent::CalculateObliqueMatrix(Maths::Vec4 clipPlane)
	{
		Maths::Mat4 projectionMatrix = GetProjectionMatrix();
		Maths::Vec4 q = projectionMatrix.inverse * Maths::Vec4(sgn(clipPlane.x), sgn(clipPlane.y), 1.0f, 1.0f);
		Maths::Vec4 c = clipPlane * (2.0F / (Maths::Dot (clipPlane, q)));
		// third row = clip plane - fourth row
		projectionMatrix[0][2] = c.x - projectionMatrix[0][3];
		projectionMatrix[1][2] = c.y - projectionMatrix[1][3];
		projectionMatrix[2][2] = c.z - projectionMatrix[2][3];
		projectionMatrix[3][2] = c.w - projectionMatrix[3][3];
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

	Maths::Vec3 CameraComponent::ScreenToViewportPoint(Maths::Vec3 position)
	{
		float nx = (position.x - viewportPos.x) / viewportSize.x;
		float ny = (position.y - viewportPos.y) / viewportSize.y;
		return Maths::Vec3(nx, ny, position.z);
	}

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

	Maths::Vec3 CameraComponent::ViewportToScreenPoint(Maths::Vec3 position)
	{
		float nx = position.x * viewportSize.x + viewportPos.x;
		float ny = position.y * viewportSize.y + viewportPos.y;
		return Maths::Vec3(nx, ny, position.z);
	}

	Maths::Vec3 CameraComponent::ViewportToWorldPoint(Maths::Vec3 position)
	{
		Maths::Vec3 screenPoint = ViewportToScreenPoint (position);
		return ScreenToWorldPoint (screenPoint);
	}

	Maths::Vec3 CameraComponent::WorldToScreenPoint(Maths::Vec3 position)
	{
		Maths::Vec3 o;
		Maths::Mat4 clipToWorld = GetClipToWorldMatrix();
		CameraProject( position, GetCameraToWorldMatrix(), clipToWorld, viewportSize, o);
		return o;
	}

	Maths::Vec3 CameraComponent::WorldToViewportPoint(Maths::Vec3 position)
	{
		Maths::Vec3 screenPoint = WorldToScreenPoint (position);
		return ScreenToViewportPoint (screenPoint);
	}

	Maths::Vec4 CameraComponent::GetScreenViewportRect()
	{
		return Maths::Vec4(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);
	}

	Maths::Mat4 CameraComponent::GetWorldToClipMatrix()
	{
		return (GetProjectionMatrix() * GetWorldToCameraMatrix());
	}

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

	Maths::Mat4 CameraComponent::GetWorldToCameraMatrix()
	{
		Maths::Mat4 worldToCameraMatrix;
		worldToCameraMatrix.SetScaleMatrix(Maths::Vec3(1.0F, 1.0F, -1.0F));
		worldToCameraMatrix = worldToCameraMatrix * transform.GetWorldToLocalMatrixNoScale();
		return worldToCameraMatrix;
	}

	Maths::Mat4 CameraComponent::GetCameraToWorldMatrix()
	{
		Maths::Mat4 m = GetWorldToCameraMatrix();
		m.Invert_Full();
		return m;
	}

	Maths::Mat4 CameraComponent::GetClipToWorldMatrix()
	{
		return Maths::Mat4.Invert_Full(GetWorldToClipMatrix());
	}

	bool CameraComponent::CameraProject(Maths::Vec3 p, Maths::Mat4 cameraToWorld, Maths::Mat4 worldToClip,
			Maths::Vec2 viewport, Maths::Vec3& outP)
	{
		Maths::Vec3 clipPoint;
		outP = Maths::Vec3(0, 0, 0);
		if( worldToClip.PerspectiveMultiplyPoint3( p, out clipPoint ) )
		{
			Maths::Vec3 cameraPos = cameraToWorld.GetPosition();
			Maths::Vec3 dir = p - cameraPos;
			// The camera/projection matrices follow OpenGL convention: positive Z is towards the viewer.
			// So negate it to get into Unity convention.
			Maths::Vec3 forward = -cameraToWorld.GetAxisZ();
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
		if( clipToWorld.PerspectiveMultiplyPoint3( in_v, out pointOnPlane ) )
		{
			// Now we have a point on the plane perpendicular to the viewing direction. We need to return the one that is on the line
			// towards this point, and at p.z distance along camera's viewing axis.
			Maths::Vec3 cameraPos = cameraToWorld.GetPosition();
			Maths::Vec3 dir = pointOnPlane - cameraPos;

			// The camera/projection matrices follow OpenGL convention: positive Z is towards the viewer.
			// So negate it to get into Unity convention.
			Maths::Vec3 forward = -cameraToWorld.GetAxisZ();
			float distToPlane = Maths::Dot( dir, forward );
			if( abs(distToPlane) >= 1.0e-6f )
			{
				bool isPerspective = clipToWorld.IsPerspective();
				if( isPerspective )
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
}