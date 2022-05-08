//
// Created by Matt on 26/04/2022.
//

#pragma once

#include <ComponentComposition/Component.h>
#include <ComponentComposition/Transform.h>
#include <ParticleEffects/ParticleEmitter.h>
#include <ParticleEffects/ParticleRenderer.h>
#include <OpenGL/Texture.h>

namespace PlatinumEngine
{
	class ParticleEffect : public Component
	{
	public:
		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		ParticleEffect();
		void OnUpdate(Scene& scene, double deltaTime) override;
		void OnRender(Scene& scene, Renderer& renderer) override;
		void OnIDSystemUpdate(Scene& scene) override;

	public:
		ParticleEffects::ParticleEmitter particleEmitter;

		// Values which the particle emitter does not need to know but are user define-able options
		bool useTexture = false;
		std::string shadeBy = "Life";
		// last frame number that this rendered. Used to avoid incrementing the deltaTime multiple times per frame.
		int lastFrame = -1;

		// TODO:
		//  Adding interpolation between more than one colour
		//  Bezier curve interpolation schemes?
		Maths::Vec4 startColour = Maths::Vec4(1,0,0,1);
		Maths::Vec4 endColour = Maths::Vec4(0,1,0,0);
		float minShadeValue = 0.0f;
		float maxShadeValue = 4.0f;
	};
}