//
// Created by Matt on 26/04/2022.
//

#include <ComponentComposition/ParticleEffect.h>
#include <ParticleEffects/ParticleEmitter.h>
#include <SceneManager/Scene.h>

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

		// only move deltaTime once each frame
		double deltaTime;
		if (scene.time.getFramesPassed() == lastFrame)
		{
			deltaTime = 0.f;
		}
		else
		{
			deltaTime = scene.time.GetDelta();
			lastFrame = scene.time.getFramesPassed();
		}

		particleEmitter.UpdateParticles(deltaTime, renderer.cameraPos);
		if (particleEmitter.particles)
		{
			// Bind shader stuff here
			renderer.particleRenderer.SetInput(particleEmitter.particles);
			renderer.BeginParticleShader();
			renderer.SetFloatParticleShader("maxLife", particleEmitter.respawnLifetime);
			renderer.SetVec4ParticleShader("StartColour", startColour);
			renderer.SetVec4ParticleShader("EndColour", endColour);
			renderer.SetTextureParticleShader(particleEmitter.texture, useTexture, particleEmitter.numColsInTexture, particleEmitter.numRowsInTexture);
			renderer.SetShadeByParticleShader(shadeBy);
			renderer.SetFloatParticleShader("minVal", minShadeValue);
			renderer.SetFloatParticleShader("maxVal", maxShadeValue);
			renderer.SetBoolParticleShader("isSpherical", !particleEmitter.useCylindricalBillboard);
			renderer.SetVec3ParticleShader("scaleFactors", particleEmitter.scaleFactors);
			renderer.particleRenderer.Render();
			renderer.EndParticleShader();
		}
	}

	void ParticleEffect::OnIDSystemUpdate(Scene& scene)
	{
		particleEmitter.texture.OnIDSystemUpdate(scene.idSystem);
	}
}