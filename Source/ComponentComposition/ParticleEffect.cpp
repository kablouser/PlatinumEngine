//
// Created by Matt on 26/04/2022.
//

#include <ComponentComposition/ParticleEffect.h>
#include <ParticleEffects/ParticleEmitter.h>

namespace PlatinumEngine
{
	ParticleEffect::ParticleEffect()
	{
		_particleEmitter.SetNumberOfParticles(_particleEmitter.GetNumberOfParticles());
		_particleRenderer.SetInput(_particleEmitter.GetParticles());
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
		_particleEmitter.UpdateParticles(0.016);
		_particleRenderer.SetInput(_particleEmitter.GetParticles());
		_particleRenderer.Render(renderer);
	}

	void ParticleEffect::SetNumberOfParticles(const int numParticles)
	{
		_particleEmitter.SetNumberOfParticles(numParticles);
		_particleRenderer.SetInput(_particleEmitter.GetParticles());
	}

	int ParticleEffect::GetNumberOfParticles()
	{
		return _particleEmitter.GetNumberOfParticles();
	}

	void ParticleEffect::SetRespawnLifetime(float lifetime)
	{
		_particleEmitter.SetRespawnLifetime(lifetime);
	}

	float ParticleEffect::GetRespawnLifetime()
	{
		return _particleEmitter.GetRespawnLifetime();
	}

	void ParticleEffect::SetNumberNewParticles(int numNewParticles)
	{
		_particleEmitter.SetNumberNewParticles(numNewParticles);
	}

	int ParticleEffect::GetNumberNewParticles()
	{
		return _particleEmitter.GetNumberNewParticles();
	}
}