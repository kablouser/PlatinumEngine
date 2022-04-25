//
// Created by Jinyuan on 05/04/2022.
//
#pragma once
#include <GL/glew.h>
#include <Maths/Vectors.h>
#include <OpenGL/Texture.h>
namespace PlatinumEngine
{
#define INVALID_HANDLE 0
/*
	struct MaterialRes
	{
		std::string diffuseTextureFile;
		std::string normalTextureFile;
		std::string specularTextureFile;
	};
*/

	class Material
	{
	public:
		Texture*			diffuseTexture;
		Texture*			normalTexture;
		Texture*			specularTexture;

		float         	shininessFactor = 8.0f;

		// Flags
		bool useBlinnPhong = false;
		bool useTexture = false;
		bool useNormalTexture = false;
		bool useReflectionShader = false;

		Material();
		~Material();

	};
}

