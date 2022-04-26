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
}