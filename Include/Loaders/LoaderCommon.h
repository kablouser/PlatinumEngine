//
// Created by Matt on 03/03/2022.
//

#pragma once

#include <string>
#include <vector>

// For now to avoid anything weird explicitly only allow these files to be loaded
const std::vector<std::string> ALLOWED_EXTENSIONS{"obj"};

namespace PlatinumEngine
{

	/**
	 * Returns the extensions of the given filepath
	 * @param filePath
	 * @return : Extension as string
	 */
	std::string GetExtension(const std::string& filePath);

	/**
	 * Checks is extension exists in list of allowed extensions
	 * @param extension
	 * @return : True if extensions ok, false is bad
	 */
	bool ExtensionAllowed(const std::string& extension);
}