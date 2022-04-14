//
// Created by Jinyuan on 05/04/2022.
//
#include "OpenGL/Material.h"

namespace PlatinumEngine
{
	Material::Material():shininessFactor(1.0f),
						 diffuseTexture(nullptr),
						 normalTexture(nullptr),
						 specularTexture(nullptr)
	{

	}

	Material::~Material() { }

}
