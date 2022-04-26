//
// Created by Matt on 03/03/2022.
//

#include "Loaders/MeshLoader.h"


namespace PlatinumEngine
{
	namespace Loaders
	{
		void LoadMesh(const std::string &filePath, std::vector<glm::vec3> &outPositions, std::vector<glm::vec3> &outNormals, std::vector<glm::vec2> &outTextureCoords)
		{
			// First check if the file is okay
			if (!ExtensionAllowed(GetExtension(filePath)))
			{
				PLATINUM_ERROR({"Extension for file \"" + filePath + "\" not allowed"});
				return;
			}

			// Load file
			Assimp::Importer import;
			const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
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

		Mesh LoadMesh(const std::string &filePath, const bool JoinVertices, const bool CalcTangents)
		{
			// First check if the file is okay
			if (!ExtensionAllowed(GetExtension(filePath)))
			{
				PLATINUM_ERROR({"Extension for file \"" + filePath + "\" not allowed"});
				return {};
			}

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
			ozz::io::File file(filePath.c_str(), "rb");
			if (!file.opened())
			{
				PLATINUM_WARNING("OZZ IO cannot open this file.");
			}
			ozz::io::IArchive archive(&file);

			if (!archive.TestTag<ozz::animation::Skeleton>())
			{
				PLATINUM_WARNING(std::string("Archive for ") + filePath +std::string(" doesn't contain the expected object type."));
			}

			const aiScene *scene = import.ReadFile(filePath, flags);
			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				PLATINUM_ERROR(import.GetErrorString());
				return {};
			}

			// Loop all meshes and add data
			Mesh returnMesh;
			// Keep track of offset for multiple meshes
			unsigned int offset = 0;
			for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			{
				unsigned int tempOffset = offset;
				AddMeshData(returnMesh, scene->mMeshes[i], offset);
				AddBoneData(returnMesh, scene->mMeshes[i], tempOffset);
			}



			// Get all nodes
			AddNodeData(returnMesh, scene->mRootNode);

			// Loop all animations
			for(unsigned int i = 0; i < scene->mNumAnimations; ++i)
				AddAnimationData(returnMesh, scene->mAnimations[i]);

			return returnMesh;
		}

		void AddMeshData(Mesh &outMesh, aiMesh *mesh, unsigned int &offset)
		{
			// Store offset
			outMesh.offset.push_back(offset);

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

			// Render offset for next mesh, we add 1 as indices in next mesh will start at 0
			offset += highestIndex + 1;
		}

		void AddBoneData(Mesh &outMesh, aiMesh* inMesh, unsigned int offset)
		{
			// Bond variables
			Bone bone;
			aiBone* inBone;
			unsigned int boneID = 0, numOfBones = 0;
			unsigned int offSetForBoneID = outMesh.boneMapping.size();
			outMesh.verticesBones.resize(outMesh.vertices.size());
			if(outMesh.verticesBones.empty())
			{
				PLATINUM_WARNING("There is no vertex attached to the bone.");
			}

			for(unsigned int k =0 ;k < inMesh->mNumBones; k++)
			{
				inBone = inMesh->mBones[k];

				// Name
				bone.boneName = inBone->mName.C_Str();

				// Update bone mapping
				if (outMesh.boneMapping.find(bone.boneName) == outMesh.boneMapping.end())
				{
					// Update bone id
					boneID = numOfBones;
					bone.boneID = boneID;

					// Matrix
					bone.SetTranformMatrixByaiMatrix(inBone->mOffsetMatrix);

					// Update bone map
					outMesh.boneMapping[bone.boneName] = bone;
				}
				else
				{
					boneID = outMesh.boneMapping[bone.boneName].boneID - offSetForBoneID;
				}

				// Update VertexBone info
				for(unsigned int i = 0; i < inBone->mNumWeights; ++i)
				{
					if(outMesh.verticesBones.size() > inBone->mWeights->mVertexId + offset)
						outMesh.verticesBones[inBone->mWeights->mVertexId + offset].AddBone(boneID,inBone->mWeights->mWeight);
					else
						PLATINUM_WARNING("Vertex number doesn't match the vertex id in bones info read by Assimp.");
				}
			}
		}

		void AddNodeData( Mesh &outMesh, aiNode *rootNode)
		{
			if(rootNode == nullptr)
			{
				return;
			}

			// create list to record read nodes
			std::vector<aiNode*> readNodes = { rootNode };

			// add new bone node into the list
			outMesh.nodes.emplace_back(BoneNode(readNodes[0]->mName.C_Str()));
			outMesh.nodes[0].parent = nullptr;
			outMesh.nodes[0].SetTranformMatrixByaiMatrix(readNodes[0]->mTransformation);

			for(unsigned int j = 0; j < readNodes.size(); ++j)
			{
				// add nodes into read list
				for (unsigned int i = 0; i < readNodes[j]->mNumChildren; ++i)
				{
					// add the child into read list
					readNodes.emplace_back(readNodes[j]->mChildren[i]);
					// add the child into the bone node list in mesh object
					outMesh.nodes.emplace_back(BoneNode(readNodes[j]->mChildren[i]->mName.C_Str()));
					// set parent
					outMesh.nodes.back().parent = &outMesh.nodes[j];
					// set transformation matrix
					outMesh.nodes.back().SetTranformMatrixByaiMatrix(readNodes[j]->mChildren[i]->mTransformation);
					// set mesh id
					for(unsigned int k =0; k < readNodes[j]->mChildren[i]->mNumMeshes; ++k)
						outMesh.nodes.back().meshIDs.emplace_back(readNodes[j]->mChildren[i]->mMeshes[k]);
				}

				// set children
				for (unsigned int i = 0; i < readNodes[j]->mNumChildren; i++)
				{
					&outMesh.nodes[j].children.emplace_back(&outMesh.nodes[j + i + 1]);
				}
			}
		}

		void AddAnimationData(Mesh &outMesh, aiAnimation* inAnimation)
		{
			Animation animation;

			// Basic info
			animation.animationName = inAnimation->mName.C_Str();
			animation.duration = inAnimation->mDuration;
			animation.tickPerSecond = inAnimation->mTicksPerSecond;

			// Channels
			for(unsigned int i =0; i<inAnimation->mNumChannels; ++i)
				animation.AddChannelByaiNodeAnim(inAnimation->mChannels[i]);

			outMesh.animations.emplace_back(animation);
		}
	}
}



