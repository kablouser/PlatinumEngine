//
// Created by Matt on 26/04/2022.
//

#include <ComponentComposition/ParticleEffect.h>
#include <ParticleEffects/ParticleEmitter.h>

namespace PlatinumEngine
{
	ParticleEffect::ParticleEffect()
	{
		particleEmitter = std::make_unique<ParticleEffects::ParticleEmitter>();
//		particleRenderer = std::make_unique<ParticleEffects::ParticleRenderer>();
	}

	void ParticleEffect::OnUpdate(Scene& scene, double deltaTime)
	{
		std::cout << "updating";
	}

	void ParticleEffect::OnRender(Scene& scene, Renderer& renderer)
	{
		// TODO:
		// 	Adjust initial position settings
		// 	Non-linear interpolation between colours
		// 	Textures?
		//  Noise/Different distribution algorithms, Will need new emitter classes inheriting from base emitter
		// Render all particles
		TransformComponent* transform = GetComponent<TransformComponent>();
		if (transform)
			renderer.SetModelMatrix(transform->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();

		// Manually update here for now
		particleEmitter->UpdateParticles(0.016, renderer.cameraPos);
		if (particleEmitter->particles)
		{
			// Bind shader stuff here
			renderer.particleRenderer.SetInput(particleEmitter->particles);
			renderer.BeginParticleShader();
			renderer.SetMaxLifeParticleShader(particleEmitter->respawnLifetime);
			renderer.SetStartColourParticleShader(startColour);
			renderer.SetEndColourParticleShader(endColour);
			renderer.SetTextureParticleShader(texture, useTexture);
			renderer.SetShadeByParticleShader(shadeBy);
			renderer.SetMinMaxShadeByParticleShader(minShadeValue, maxShadeValue);
			renderer.particleRenderer.Render();
			renderer.EndParticleShader();
		}
	}
}