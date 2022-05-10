//
// Created by Jinyuan on 05/04/2022.
//
#pragma once
#include <OpenGL/Texture.h>
#include <IDSystem/IDSystem.h>
#include <TypeDatabase/TypeDatabase.h>
#include <Maths/Vectors.h>

namespace PlatinumEngine
{
	class Material
	{
	public:
		SavedReference<Texture>			diffuseTexture;
		SavedReference<Texture>			normalTexture;
		SavedReference<Texture>			specularTexture;

		float         	shininessFactor = 8.0f;
		float refractionIndex = 1.5f;

		// Flags
		bool useBlinnPhong = false;
		bool useTexture = false;
		bool useNormalTexture = false;
		bool useReflectionShader = false;
		bool useRefractionShader = false;
		Maths::Vec3 colour = {1.0f, 0.0f, 0.0f};

		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		Material();
		void OnIDSystemUpdate(IDSystem& idSystem);
	};
}
