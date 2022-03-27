#ifndef PLATINUMENGINE_CAMERACOMPONENT_H
#define PLATINUMENGINE_CAMERACOMPONENT_H

#include "Maths/Vectors.h"
#include "Maths/Quaternion.h"
#include "ComponentComposition/Component.h"

namespace PlatinumEngine
{
	enum projectionType {PERSPECTIVE, ORTHOGRAPHIC};
	class CameraComponent: Component
	{
	public:
		projectionType projection;
		Maths::Vec3 backgroundColor;
		float fov;
		float aspectRatio;
		float nearClippingPlane;
		float farClippingPlane;
		float size;
	};
}

#endif //PLATINUMENGINE_CAMERACOMPONENT_H
