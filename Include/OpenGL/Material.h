//
// Created by Jinyuan on 05/04/2022.
//
#pragma once
#include <OpenGL/Texture.h>
#include <IDSystem/IDSystem.h>
namespace PlatinumEngine
{
	class Material
	{
	public:
		SavedReference<Texture>			diffuseTexture;
		SavedReference<Texture>			normalTexture;
		SavedReference<Texture>			specularTexture;

		float         	shininessFactor = 8.0f;

		// Flags
		bool useBlinnPhong = false;
		bool useTexture = false;
		bool useNormalTexture = false;

		Material();
	};
}

