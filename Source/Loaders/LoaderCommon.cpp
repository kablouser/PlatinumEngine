//
// Created by Matt on 03/03/2022.
//

#include <Loaders/LoaderCommon.h>

namespace PlatinumEngine
{
	namespace Loaders
	{
		// Instance of the global variable
		const std::map<std::string, std::type_index> EXTENSION_TO_TYPE{
				{ ".obj", std::type_index(typeid(Mesh)) },
				// .fbx?

				{ ".png", std::type_index(typeid(Texture)) },
				// .jpg? .jpeg?
		};

		bool ExtensionAllowed(const std::filesystem::path& path)
		{
			auto findExtension = EXTENSION_TO_TYPE.find(path.extension().string());
			return findExtension != EXTENSION_TO_TYPE.end();
		}

		bool IsAsset(const std::type_index& typeIndex)
		{
			for (auto& extensionAndType: EXTENSION_TO_TYPE)
			{
				if (extensionAndType.second == typeIndex)
					return true;
			}
			return false;
		}
	}
}



