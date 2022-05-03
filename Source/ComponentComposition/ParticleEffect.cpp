//
// Created by Matt on 26/04/2022.
//

#include <ComponentComposition/ParticleEffect.h>
#include <ParticleEffects/ParticleEmitter.h>

namespace PlatinumEngine
{
	ParticleEffect::ParticleEffect()
	{
		// TODO: Use OnStart/OnEnd functions
		particleEmitter = std::make_unique<ParticleEffects::ParticleEmitter>();
	}

	void ParticleEffect::OnUpdate(Scene& scene, double deltaTime)
	{
		// TODO: Update Particles here
	}

	void ParticleEffect::OnRender(Scene& scene, Renderer& renderer)
	{
		// Render all particles
		SavedReference<Transform> transform = GetComponent<Transform>();
		if (transform)
			renderer.SetModelMatrix(transform.DeRef()->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();

		// Manually update here for now
		particleEmitter->UpdateParticles(0.016, renderer.cameraPos);
		if (particleEmitter->particles)
		{
			// Bind shader stuff here
			renderer.particleRenderer.SetInput(particleEmitter->particles);
			renderer.BeginParticleShader();
			renderer.SetFloatParticleShader("maxLife", particleEmitter->respawnLifetime);
			renderer.SetVec4ParticleShader("StartColour", startColour);
			renderer.SetVec4ParticleShader("EndColour", endColour);
			renderer.SetTextureParticleShader(particleEmitter->texture, useTexture, particleEmitter->numColsInTexture, particleEmitter->numRowsInTexture);
			renderer.SetShadeByParticleShader(shadeBy);
			renderer.SetFloatParticleShader("minVal", minShadeValue);
			renderer.SetFloatParticleShader("maxVal", maxShadeValue);

			renderer.particleRenderer.Render();
			renderer.EndParticleShader();
		}
	}
}