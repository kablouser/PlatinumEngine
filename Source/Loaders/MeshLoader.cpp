//
// Created by Matt on 03/03/2022.
//

#include "Loaders/MeshLoader.h"

namespace PlatinumEngine
{
	namespace Loaders
	{
		void LoadMesh(const std::filesystem::path& filePath, std::vector<glm::vec3> &outPositions, std::vector<glm::vec3> &outNormals, std::vector<glm::vec2> &outTextureCoords)
		{
			// no point in checking extension here, just load it if possible

			// Load file
			Assimp::Importer import;
			const aiScene *scene = import.ReadFile(filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				PLATINUM_ERROR(import.GetErrorString());
				return;
			}

			// For now, take first mesh and dump into vectors
			ConvertMesh(scene->mMeshes[0], outPositions, outNormals, outTextureCoords);
		}

		void ConvertMesh(aiMesh *mesh, std::vector<glm::vec3> &outPositions, std::vector<glm::vec3> &outNormals, std::vector<glm::vec2> &outTextureCoords)
		{
			// Loop vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				outPositions.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
				outNormals.emplace_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
				if (mesh->mTextureCoords[0])
				{
					outTextureCoords.emplace_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
				}
				else
				{
					outTextureCoords.emplace_back(glm::vec2(0.0f, 0.0f));
				}
			}
		}

		std::pair<bool, Mesh> LoadMesh(const std::filesystem::path& filePath, const bool JoinVertices, const bool CalcTangents)
		{
			// no point in checking extension here, just load it if possible

			// Load file
			Assimp::Importer import;
			unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_MakeLeftHanded;
			if (JoinVertices)
			{
				flags = flags | aiProcess_JoinIdenticalVertices;
			}
			if (CalcTangents)
			{
				flags = flags | aiProcess_CalcTangentSpace;
			}

			// Crate scene from file
			const aiScene *scene = import.ReadFile(filePath.string(), flags);
			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				PLATINUM_ERROR(import.GetErrorString());
				return {false, {}};
			}

			// Loop all meshes and add data
			Mesh returnMesh;
			returnMesh.fileName = filePath.filename().string();
			// Keep track of offset for multiple meshes
			unsigned int offset = 0;
			for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			{
				AddMeshData(returnMesh, scene->mMeshes[i], offset);
			}

			return {true, std::move(returnMesh)};
		}

		void AddMeshData(Mesh &outMesh, aiMesh *mesh, unsigned int &offset)
		{
			// Keep track of the highest index of this mesh
			unsigned int highestIndex = 0;

			// Loop vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				// Create a vertex which stores position, normal and texture coordinates
				Vertex vertex{};
				vertex.position.x = mesh->mVertices[i].x;
				vertex.position.y = mesh->mVertices[i].y;
				vertex.position.z = mesh->mVertices[i].z;
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;
				if (mesh->mTextureCoords[0])
				{
					vertex.textureCoords.x = mesh->mTextureCoords[0][i].x;
					vertex.textureCoords.y = mesh->mTextureCoords[0][i].y;
				}
				else
				{
					vertex.textureCoords.x = 0.0f;
					vertex.textureCoords.y = 0.0f;
				}
				if (mesh->HasTangentsAndBitangents())
				{
					vertex.tangent.x = mesh->mTangents[i].x;
					vertex.tangent.y = mesh->mTangents[i].y;
					vertex.tangent.z = mesh->mTangents[i].z;
					vertex.biTangent.x = mesh->mBitangents[i].x;
					vertex.biTangent.y = mesh->mBitangents[i].y;
					vertex.biTangent.z = mesh->mBitangents[i].z;
				}
				else
				{
					vertex.tangent.x = 0.0f;
					vertex.tangent.y = 0.0f;
					vertex.tangent.z = 0.0f;
					vertex.biTangent.x = 0.0f;
					vertex.biTangent.y = 0.0f;
					vertex.biTangent.z = 0.0f;
				}

				outMesh.vertices.emplace_back(vertex);
			}

			// Loop indices of the faces of the mesh
			for(unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for(unsigned int j = 0; j < face.mNumIndices; j++)
				{
					outMesh.indices.push_back(offset + face.mIndices[j]);

					// Check for new highest
					if (face.mIndices[j] > highestIndex)
						highestIndex = face.mIndices[j];
				}
			}

			// Update offset for next mesh, we add 1 as indices in next mesh will start at 0
			offset += highestIndex + 1;
		}
	}
}



