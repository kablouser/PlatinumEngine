//
// Created by Matt on 26/04/2022.
//

#include <ComponentComposition/ParticleEffect.h>
#include <ParticleEffects/ParticleEmitter.h>
#include <SceneManager/Scene.h>
#include <Application.h>

namespace PlatinumEngine
{
	void ParticleEffect::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<ParticleEffect>()
				.WithInherit<Component>()
				.WithField<bool>("useTexture", PLATINUM_OFFSETOF(ParticleEffect, useTexture))
				.WithField<std::string>("shadeBy", PLATINUM_OFFSETOF(ParticleEffect, shadeBy))
				.WithField<Maths::Vec4>("startColour", PLATINUM_OFFSETOF(ParticleEffect, startColour))
				.WithField<Maths::Vec4>("endColour", PLATINUM_OFFSETOF(ParticleEffect, endColour))
				.WithField<float>("minShadeValue", PLATINUM_OFFSETOF(ParticleEffect, minShadeValue))
				.WithField<float>("maxShadeValue", PLATINUM_OFFSETOF(ParticleEffect, maxShadeValue))
				.WithField<ParticleEffects::ParticleEmitter>("particleEmitter", PLATINUM_OFFSETOF(ParticleEffect, particleEmitter));
	}

	ParticleEffect::ParticleEffect()
	{
		// TODO: Use OnStart/OnEnd functions
	}

	void ParticleEffect::OnUpdate()
	{
		// TODO: Update Particles here
	}

	void ParticleEffect::OnRender()
	{
		// Render all particles
		SavedReference<Transform> transform = GetComponent<Transform>();
		if (transform)
			Application::Instance->renderer.SetModelMatrix(transform.DeRef()->GetLocalToWorldMatrix());
		else
			Application::Instance->renderer.SetModelMatrix();

		// only move deltaTime once each frame
		double deltaTime;
		if (Application::Instance->time.getFramesPassed() == lastFrame)
		{
			deltaTime = 0.f;
		}
		else
		{
			deltaTime = Application::Instance->time.GetDelta();
			lastFrame = Application::Instance->time.getFramesPassed();
		}

		particleEmitter.UpdateParticles(deltaTime, Application::Instance->renderer.cameraPos);
		if (particleEmitter.particles)
		{
			// Bind shader stuff here
			Application::Instance->renderer.particleRenderer.SetInput(particleEmitter.particles);
			Application::Instance->renderer.BeginParticleShader();
			Application::Instance->renderer.SetFloatParticleShader("maxLife", particleEmitter.respawnLifetime);
			Application::Instance->renderer.SetVec4ParticleShader("StartColour", startColour);
			Application::Instance->renderer.SetVec4ParticleShader("EndColour", endColour);
			Application::Instance->renderer.SetTextureParticleShader(particleEmitter.texture, useTexture, particleEmitter.numColsInTexture, particleEmitter.numRowsInTexture);
			Application::Instance->renderer.SetShadeByParticleShader(shadeBy);
			Application::Instance->renderer.SetFloatParticleShader("minVal", minShadeValue);
			Application::Instance->renderer.SetFloatParticleShader("maxVal", maxShadeValue);

			Application::Instance->renderer.particleRenderer.Render();
			Application::Instance->renderer.EndParticleShader();
		}
	}

	void ParticleEffect::OnIDSystemUpdate()
	{
		particleEmitter.texture.OnIDSystemUpdate(Application::Instance->idSystem);
	}
}