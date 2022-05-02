//
// Created by Matt on 03/03/2022.
//

#pragma once

#include <string>
#include <vector>


namespace PlatinumEngine
{
	// TODO change extensions to ".obj" instead of just "obj"
	// because C++ filesystem get extensions returns ".obj" so it's easier to use
	// also create a enum extensions allowed, makes programming less error prone

	// For now to avoid anything weird explicitly only allow these files to be loaded
	static const std::vector<std::vector<std::string>> ALLOWED_EXTENSIONS {{ "fbx" , "obj"}, { "wav" }, { "png" }};

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
