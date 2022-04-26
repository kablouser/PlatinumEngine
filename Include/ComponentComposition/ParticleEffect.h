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
		void SetNumberOfParticles(int numParticles);
		int GetNumberOfParticles();
		void SetRespawnLifetime(float lifetime);
		float GetRespawnLifetime();
		void SetNumberNewParticles(int numNewParticles);
		int GetNumberNewParticles();
	private:
		ParticleEffects::ParticleEmitter _particleEmitter;
		ParticleEffects::ParticleRenderer _particleRenderer;
	};
}