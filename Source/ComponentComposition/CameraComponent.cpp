#include "ComponentComposition/CameraComponent.h"

namespace PlatinumEngine
{
    CameraComponent::CameraComponent()
    {
         projection = ProjectionType::perspective;
		 backgroundColor = Color(0.f,0.f,0.f,1.f);
		 fov = 60;
		 nearClippingPlane = 0.3f;
		 farClippingPlane = 1000;
		 othographicSize = 1;
		 viewportSize = Maths::Vec2(1.f,1.f);
		 viewportPos = Maths::Vec2(0.f,0.f);
		 clearFlag = ClearFlagType::none;
    }
	CameraComponent::~CameraComponent()
	{}

	//Transform point from screen to world coordinates
	Maths::Vec3 CameraComponent::ScreenToWorldPoint(Maths::Vec3 position)
	{
		Maths::Vec4 viewport(viewportPos.x,viewportPos.y,viewportSize.x,viewportSize.y);
		return CameraUnProject(position, GetWorldToCameraMatrix(), GetProjectionMatrix(), viewport);
	}
	//Transform point from world to screen coordinates
	Maths::Vec3 CameraComponent::WorldToScreenPoint(Maths::Vec3 position)
	{
		Maths::Vec4 viewport(viewportPos.x,viewportPos.y,viewportSize.x,viewportSize.y);
		return CameraProject(position, GetWorldToCameraMatrix(), GetProjectionMatrix(), viewport);
	}
	//Returns matrix that convert from world space to clip space (View*Projection matrix)
	Maths::Mat4 CameraComponent::GetWorldToClipMatrix()
	{
		return (GetProjectionMatrix() * GetWorldToCameraMatrix());
	}
	//Returns matrix that convert from clip space to world space (Inverse of View*Projection matrix)
	Maths::Mat4 CameraComponent::GetClipToWorldMatrix()
	{
		return Maths::Inverse(GetWorldToClipMatrix());
	}
	//Returns the Projection matrix
	Maths::Mat4 CameraComponent::GetProjectionMatrix()
	{
		float aspectRatio = viewportSize.x / viewportSize.y;
		Maths::Mat4 projectionMatrix;
		if (projection==ProjectionType::perspective)
			projectionMatrix.SetPerspectiveMatrix( fov, aspectRatio, nearClippingPlane, farClippingPlane );
		else
			projectionMatrix.SetOrthogonalMatrix( -othographicSize * aspectRatio, othographicSize * aspectRatio, -othographicSize, othographicSize, nearClippingPlane, farClippingPlane );
		return projectionMatrix;
	}
	//Returns matrix that convert from world coordinates to camera coordinates (View matrix)
	Maths::Mat4 CameraComponent::GetWorldToCameraMatrix()
	{
		Maths::Mat4 worldToCameraMatrix;
		TransformComponent* tc = GetGameObject()->GetComponent<TransformComponent>();
		if(tc != nullptr)
		{
			Maths::Mat4 invR, invT;
			invR = Maths::Quaternion::QuaternionToMatrix(Maths::Quaternion::Inverse(tc->rotation));
			invT.SetTranslationMatrix(-tc->position);
			worldToCameraMatrix = invR*invT;
		}
		return worldToCameraMatrix;
	}
	//Returns matrix that convert from camera coordinates to world coordinates (Inverse View matrix)
	Maths::Mat4 CameraComponent::GetCameraToWorldMatrix()
	{
		Maths::Mat4 cameraToWorldMatrix;
		TransformComponent* tc = GetGameObject()->GetComponent<TransformComponent>();
		if(tc != nullptr)
		{
			Maths::Mat4 t,r;
			t.SetTranslationMatrix(tc->position);
			r = Maths::Quaternion::QuaternionToMatrix(tc->rotation);
			cameraToWorldMatrix = t*r;
		}
		return cameraToWorldMatrix;
	}
	//Projects 3D space onto the 2D screen
	Maths::Vec3 CameraComponent::CameraProject(Maths::Vec3 obj, Maths::Mat4 modelview, Maths::Mat4 proj, Maths::Vec4 viewport)
	{
		glm::vec3 o = obj;
		glm::vec4 vp = viewport;
		glm::mat4 m,p;

		m[0][0]=modelview[0][0]; m[0][1]=modelview[0][1]; m[0][2]=modelview[0][2]; m[0][3]=modelview[0][3];
		m[1][0]=modelview[1][0]; m[1][1]=modelview[1][1]; m[1][2]=modelview[1][2]; m[1][3]=modelview[1][3];
		m[2][0]=modelview[2][0]; m[2][1]=modelview[2][1]; m[2][2]=modelview[2][2]; m[2][3]=modelview[2][3];
		m[3][0]=modelview[3][0]; m[3][1]=modelview[3][1]; m[3][2]=modelview[3][2]; m[3][3]=modelview[3][3];

		p[0][0]=proj[0][0]; p[0][1]=proj[0][1]; p[0][2]=proj[0][2]; p[0][3]=proj[0][3];
		p[1][0]=proj[1][0]; p[1][1]=proj[1][1]; p[1][2]=proj[1][2]; p[1][3]=proj[1][3];
		p[2][0]=proj[2][0]; p[2][1]=proj[2][1]; p[2][2]=proj[2][2]; p[2][3]=proj[2][3];
		p[3][0]=proj[3][0]; p[3][1]=proj[3][1]; p[3][2]=proj[3][2]; p[3][3]=proj[3][3];

		glm::vec3 wincoords = glm::project(o,m,p,vp);
		return Maths::Vec3(wincoords.x,wincoords.y,wincoords.z);
	}
	//UnProjects 3D space from the 2D screen
	Maths::Vec3 CameraComponent::CameraUnProject(Maths::Vec3 win, Maths::Mat4 modelview, Maths::Mat4 proj, Maths::Vec4 viewport)
	{
		glm::vec3 w = win;
		glm::vec4 vp = viewport;
		glm::mat4 m,p;

		m[0][0]=modelview[0][0]; m[0][1]=modelview[0][1]; m[0][2]=modelview[0][2]; m[0][3]=modelview[0][3];
		m[1][0]=modelview[1][0]; m[1][1]=modelview[1][1]; m[1][2]=modelview[1][2]; m[1][3]=modelview[1][3];
		m[2][0]=modelview[2][0]; m[2][1]=modelview[2][1]; m[2][2]=modelview[2][2]; m[2][3]=modelview[2][3];
		m[3][0]=modelview[3][0]; m[3][1]=modelview[3][1]; m[3][2]=modelview[3][2]; m[3][3]=modelview[3][3];

		p[0][0]=proj[0][0]; p[0][1]=proj[0][1]; p[0][2]=proj[0][2]; p[0][3]=proj[0][3];
		p[1][0]=proj[1][0]; p[1][1]=proj[1][1]; p[1][2]=proj[1][2]; p[1][3]=proj[1][3];
		p[2][0]=proj[2][0]; p[2][1]=proj[2][1]; p[2][2]=proj[2][2]; p[2][3]=proj[2][3];
		p[3][0]=proj[3][0]; p[3][1]=proj[3][1]; p[3][2]=proj[3][2]; p[3][3]=proj[3][3];

		glm::vec3 objcoords = glm::unProject(w,m,p,vp);
		return Maths::Vec3(objcoords.x,objcoords.y,objcoords.z);
	}
}