//
// Created by Jinyuan on 05/04/2022.
//
#include "OpenGL/Material.h"

namespace PlatinumEngine
{
	void Material::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		/*
		SavedReference<Texture>			diffuseTexture;
		SavedReference<Texture>			normalTexture;
		SavedReference<Texture>			specularTexture;
		float         	shininessFactor = 8.0f;
		bool useBlinnPhong = false;
		bool useTexture = false;
		bool useNormalTexture = false;
		 */
		typeDatabase.BeginTypeInfo<Material>()
		        .WithField<SavedReference<Texture>>("diffuseTexture", PLATINUM_OFFSETOF(Material, diffuseTexture))
				.WithField<SavedReference<Texture>>("normalTexture", PLATINUM_OFFSETOF(Material, normalTexture))
				.WithField<SavedReference<Texture>>("specularTexture", PLATINUM_OFFSETOF(Material, specularTexture))

				.WithField<float>("shininessFactor", PLATINUM_OFFSETOF(Material, shininessFactor))

				.WithField<bool>("useBlinnPhong", PLATINUM_OFFSETOF(Material, useBlinnPhong))
				.WithField<bool>("useTexture", PLATINUM_OFFSETOF(Material, useTexture))
				.WithField<bool>("useNormalTexture", PLATINUM_OFFSETOF(Material, useNormalTexture));
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
