//
// Created by Matt on 03/03/2022.
//

#include "Common/Load.h"

namespace PlatinumEngine
{
	namespace Common
	{
		void LoadMesh(const std::string &filePath, std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &textureCoords)
		{
			// First check if the file is okay
			if (!ExtensionAllowed(GetExtension(filePath)))
			{
				// TODO:
				// LOG ERROR
				std::cout << "Extension for file \"" << filePath << "\" not allowed" << std::endl;
				return;
			}

			// Load file
			Assimp::Importer import;
			const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				// TODO:
				// LOG ERROR
				std::cout << import.GetErrorString() << std::endl;
				return;
			}

			// For now, take first mesh and dump into vectors
			ConvertMesh(scene->mMeshes[0], positions, normals, textureCoords);
		}

		std::string GetExtension(const std::string &filePath)
		{
			return filePath.substr(filePath.find_last_of(".") + 1);
		}

		bool ExtensionAllowed(const std::string &extension)
		{
			std::cout << extension << std::endl;
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

		void ConvertMesh(aiMesh *mesh, std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &textureCoords)
		{
			// Loop vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				positions.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
				normals.emplace_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
				if (mesh->mTextureCoords[0])
				{
					textureCoords.emplace_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
				}
				else
				{
					textureCoords.emplace_back(glm::vec2(0.0f, 0.0f));
				}
			}
		}
	}
}



