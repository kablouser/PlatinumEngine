//
// Created by Matt on 26/04/2022.
//

#include <ComponentComposition/ParticleEffect.h>
#include <ParticleEffects/ParticleEmitter.h>

namespace PlatinumEngine
{
	ParticleEffect::ParticleEffect()
	{
		particleRenderer.SetInput(particleEmitter.GetParticles());
	}
	void ParticleEffect::OnUpdate(Scene& scene, double deltaTime)
	{
		std::cout << "updating";
	}

	void ParticleEffect::OnRender(Scene& scene, Renderer& renderer)
	{
		// Render all particles
		TransformComponent* transform = GetComponent<TransformComponent>();
		if (transform)
			renderer.SetModelMatrix(transform->GetLocalToWorldMatrix());
		else
			renderer.SetModelMatrix();

		// Manually update here for now
		particleEmitter.UpdateParticles(0.016);
		particleRenderer.SetInput(particleEmitter.GetParticles());

		// Bind shader stuff here
		renderer.BeginParticleShader();
		renderer.SetMaxLifeParticleShader(particleEmitter.respawnLifetime);
		renderer.SetStartColourParticleShader(particleEmitter.startColour);
		renderer.SetEndColourParticleShader(particleEmitter.endColour);
		particleRenderer.Render(renderer);
		renderer.EndParticleShader();
	}
}