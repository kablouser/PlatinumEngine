#pragma once

#include <ComponentComposition/Component.h>

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
