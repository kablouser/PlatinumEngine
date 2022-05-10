//
// Created by Jinyuan on 05/04/2022.
//
#include "OpenGL/Material.h"

namespace PlatinumEngine
{
	void Material::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<Material>()
		        .WithField<SavedReference<Texture>>("diffuseTexture", PLATINUM_OFFSETOF(Material, diffuseTexture))
				.WithField<SavedReference<Texture>>("normalTexture", PLATINUM_OFFSETOF(Material, normalTexture))
				.WithField<SavedReference<Texture>>("specularTexture", PLATINUM_OFFSETOF(Material, specularTexture))

				.WithField<float>("shininessFactor", PLATINUM_OFFSETOF(Material, shininessFactor))
				.WithField<float>("refractionIndex", PLATINUM_OFFSETOF(Material, refractionIndex))

				.WithField<bool>("useBlinnPhong", PLATINUM_OFFSETOF(Material, useBlinnPhong))
				.WithField<bool>("useTexture", PLATINUM_OFFSETOF(Material, useTexture))
				.WithField<bool>("useNormalTexture", PLATINUM_OFFSETOF(Material, useNormalTexture))
				.WithField<bool>("useReflectionShader", PLATINUM_OFFSETOF(Material, useReflectionShader))
				.WithField<bool>("useRefractionShader", PLATINUM_OFFSETOF(Material, useRefractionShader))
				.WithField<Maths::Vec3>("colour", PLATINUM_OFFSETOF(Material, colour));
	}

	Material::Material() : shininessFactor(1.0f), useBlinnPhong(false), useTexture(false), useNormalTexture(false)
	{
	}

	void Material::OnIDSystemUpdate(IDSystem& idSystem)
	{
		diffuseTexture.OnIDSystemUpdate(idSystem);
		normalTexture.OnIDSystemUpdate(idSystem);
		specularTexture.OnIDSystemUpdate(idSystem);
	}
}
