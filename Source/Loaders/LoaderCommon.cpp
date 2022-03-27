//
// Created by Matt on 03/03/2022.
//

#include "Loaders/LoaderCommon.h"

namespace PlatinumEngine
{
	std::string GetExtension(const std::string &filePath)
	{
		return filePath.substr(filePath.find_last_of(".") + 1);
	}

	bool ExtensionAllowed(const std::string &extension)
	{
		bool isAllowed = false;
		for (unsigned int i = 0; i < ALLOWED_EXTENSIONS.size(); ++i)
		{
			if (ALLOWED_EXTENSIONS[i] == extension)
			{
				isAllowed = true;
				break;
			}
		}
		return isAllowed;
	}
}



