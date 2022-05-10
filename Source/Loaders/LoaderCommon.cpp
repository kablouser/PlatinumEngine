//
// Created by Matt on 03/03/2022.
//

#include <Loaders/LoaderCommon.h>
#include <OpenGL/Mesh.h>
#include <OpenGL/Texture.h>
#include <Audio/AudioClip.h>

namespace PlatinumEngine
{
	namespace Loaders
	{
		// Instance of the global variable
		const std::map<std::string, std::type_index> EXTENSION_TO_TYPE{
				{ ".obj", std::type_index(typeid(Mesh)) },
				{ ".fbx", std::type_index(typeid(Mesh)) },
				{ ".FBX", std::type_index(typeid(Mesh)) },

				{ ".png", std::type_index(typeid(Texture)) },
				// .jpg? .jpeg?

				{ ".wav", std::type_index(typeid(AudioClip)) },
				{ ".aiff", std::type_index(typeid(AudioClip)) },
				{ ".riff", std::type_index(typeid(AudioClip)) },
				{ ".ogg", std::type_index(typeid(AudioClip)) },
				{ ".voc", std::type_index(typeid(AudioClip)) },
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



