//
// Created by Matt on 26/04/2022.
//

#include <ComponentComposition/ParticleEffect.h>

namespace PlatinumEngine
{
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
}