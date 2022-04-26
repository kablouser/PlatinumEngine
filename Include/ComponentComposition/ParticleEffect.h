//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/TransformComponent.h>
#include <ParticleEffects/ParticleEmitter.h>
#include <ParticleEffects/ParticleRenderer.h>

namespace PlatinumEngine
{
	class ParticleEffect : public Component
	{
	public:
		ParticleEffect();

		void OnUpdate(Scene& scene, double deltaTime) override;
		void OnRender(Scene& scene, Renderer& renderer) override;
	public:
		ParticleEffects::ParticleEmitter particleEmitter;
		ParticleEffects::ParticleRenderer particleRenderer;
	};
}