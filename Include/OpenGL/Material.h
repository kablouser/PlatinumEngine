//
// Created by Jinyuan on 05/04/2022.
//
#pragma once
#include <OpenGL/Texture.h>
#include <IDSystem/IDSystem.h>
#include <TypeDatabase/TypeDatabase.h>
namespace PlatinumEngine
{
	class Material
	{
	public:
		SavedReference<Texture>			diffuseTexture;
		SavedReference<Texture>			normalTexture;
		SavedReference<Texture>			specularTexture;

		float         	shininessFactor;

		// Flags
		bool useBlinnPhong;
		bool useTexture;
		bool useNormalTexture;

		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		Material();
	};
}

