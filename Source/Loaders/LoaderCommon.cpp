//
// Created by Matt on 03/03/2022.
//

#include <Loaders/LoaderCommon.h>

namespace PlatinumEngine
{
	namespace Loaders
	{
		bool ExtensionAllowed(const std::filesystem::path& path)
		{
			auto findExtension = EXTENSION_TO_TYPE.find(path.extension().string());
			return findExtension != EXTENSION_TO_TYPE.end();
		}
	}
}



