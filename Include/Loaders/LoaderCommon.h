//
// Created by Matt on 03/03/2022.
//

#pragma once

#include <string>
#include <vector>
#include <map>
#include <typeindex>
#include <filesystem>

#include <Logger/Logger.h>
#include <OpenGL/Mesh.h>
#include <OpenGL/Texture.h>

namespace PlatinumEngine
{
	namespace Loaders
	{
		static const std::map<std::string, std::type_index> EXTENSION_TO_TYPE{
				{ ".obj", std::type_index(typeid(Mesh)) },
				// .fbx?

				{ ".png", std::type_index(typeid(Texture)) },
				// .jpg? .jpeg?
		};

		/**
		 * Checks if path with its extension is allowed for loading
		 * @param path filesystem path
		 * @return : True if extensions ok, false is bad
		 */
		bool ExtensionAllowed(const std::filesystem::path& path);
	}
}
