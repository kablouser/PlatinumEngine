#ifndef PLATINUMENGINE_COMPONENTTYPE_H
#define PLATINUMENGINE_COMPONENTTYPE_H

#include "ComponentComposition/Component.h"

namespace PlatinumEngine
{
	class MeshComponent : public PlatinumEngine::Component
	{
	public:
		MeshComponent();
	};
	class LightComponent : public PlatinumEngine::Component
	{
	public:
		LightComponent();
	};

	class CameraComponent : public PlatinumEngine::Component
	{
	public:
		CameraComponent();
	};

}

#endif //PLATINUMENGINE_COMPONENTTYPE_H
