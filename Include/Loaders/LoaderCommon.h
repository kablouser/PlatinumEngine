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
		// 1 global variable visible to everyone
		extern const std::map<std::string, std::type_index> EXTENSION_TO_TYPE;

		/**
		 * Checks if path with its extension is allowed for loading
		 * @param path filesystem path
		 * @return : True if extensions ok, false is bad
		 */
		bool ExtensionAllowed(const std::filesystem::path& path);

		/**
		 * Checks if a given type index is an asset that can be loaded in.
		 * @param typeIndex type is check
		 * @return true if type is an asset, false otherwise
		 */
		bool IsAsset(const std::type_index& typeIndex);
	}
}
