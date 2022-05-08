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
				PLATINUM_ERROR_STREAM << "Failed to load mesh at path: " << filePath <<
					". Error: " << import.GetErrorString();
				return {false, Mesh()};
			}

			// Loop all meshes and add data
			Mesh returnMesh;
			returnMesh.fileName = filePath.filename().string();
			// Keep track of offset for multiple meshes
			unsigned int offset = 0;

			if(scene->mNumAnimations > 0)
			{
				//------------------------------
				// SKELETON (Based on nothing)
				//------------------------------

				// Store the raw skeleton hierarchy
				ozz::animation::offline::RawSkeleton rawSkeleton;
				rawSkeleton.roots.resize(1);
				AddSkeletonData(scene->mRootNode,  &rawSkeleton.roots[0]);
				// Turn skeleton hierarchy structure into runtime format
				{
					ozz::animation::offline::SkeletonBuilder skeletonBuilder;
					returnMesh.skeleton = skeletonBuilder(rawSkeleton);
				}

				//---------------------------------------------
				// Bone For Every Vertex (Based on skeleton)
				//---------------------------------------------

				// Mapping bones' name and id
				std::map<std::string, unsigned int> boneMapping;

				// update mesh and bone data for every vertex
				for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
				{
					unsigned int tempOffset = offset;
					// load vertices data in current mesh
					AddAnimationMeshData(returnMesh, scene->mMeshes[i], offset);
					// load bone data for vertices in current mesh
					AddBoneData(returnMesh, scene->mMeshes[i], scene->mAnimations[0], tempOffset, boneMapping);
				}

				//---------------------------------------------
				// ANIMATION (channel data, based on skeleton)
				//---------------------------------------------

				// Loop all animations
				for(unsigned int i = 0; i < scene->mNumAnimations; ++i)
				{
					// Load animation data (contain the channel data)
					AddAnimationChannelData(returnMesh, scene->mAnimations[i]);
				}
			}
			else
			{
				// load all vertices in all meshes
				for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
				{
					AddMeshData(returnMesh, scene->mMeshes[i], offset);
				}
			}

			return {true, std::move(returnMesh)};
		}

		void AddMeshData(Mesh &outMesh, aiMesh *mesh, unsigned int &offset)
		{
			// Store offset

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

		void AddAnimationMeshData(Mesh &outMesh, aiMesh *mesh, unsigned int &offset)
		{
			// Keep track of the highest index of this mesh
			unsigned int highestIndex = 0;

			// Loop vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				// Create a vertex which stores position, normal and texture coordinates
				AnimationVertex vertex{};
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

				outMesh.animationVertices.emplace_back(vertex);
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


		void AddBoneData(Mesh &outMesh, aiMesh* inMesh, aiAnimation* inAnimation, unsigned int offset, std::map<std::string, unsigned int>& boneMapping)
		{
			// Bond variables
			Bone bone;
			aiBone* inBone;
			unsigned int boneID = outMesh.bones.size();



			if(outMesh.animationVertices.empty())
			{
				PLATINUM_WARNING("There is no vertex.");
			}

			for(unsigned int k =0 ;k < inMesh->mNumBones; ++k)
			{
				inBone = inMesh->mBones[k];

				// Name
				bone.boneName = inBone->mName.C_Str();

				// Update bone mapping
				if (boneMapping.find(bone.boneName) == boneMapping.end())
				{
					// Update bone map
					boneMapping[bone.boneName] = boneID;

					// Update ids
					boneID++;
					FindChanelID(bone.boneName, outMesh.skeleton.get(), bone.trackID);

					// Matrix
					bone.SetTranformMatrixByaiMatrix(inBone->mOffsetMatrix);
					outMesh.bones.push_back(bone);
				}
				else
				{
					bone.trackID = outMesh.bones[boneMapping[bone.boneName]].trackID;
				}

				// Update bone info for every vertex
				for(unsigned int i = 0; i < inBone->mNumWeights; ++i)
				{
					if(outMesh.animationVertices.size() > inBone->mWeights[i].mVertexId + offset)
						outMesh.animationVertices[inBone->mWeights[i].mVertexId + offset].AddTrack(bone.trackID,inBone->mWeights[i].mWeight);
					else
						PLATINUM_WARNING("Vertex number doesn't match the vertex id in bones info read by Assimp.");
				}
			}
		}

		void AddSkeletonData(aiNode* inNode, ozz::animation::offline::RawSkeleton::Joint* currentJoint)
		{
			if(inNode == nullptr)
			{
				return;
			}

			currentJoint->name = inNode->mName.C_Str();

			// update children info
			aiVector3t<float> scaling;
			aiQuaterniont<float> rotation;
			aiVector3t<float> position;


			inNode->mTransformation.Decompose(scaling, rotation, position);
			currentJoint->transform.scale.x = scaling.x;
			currentJoint->transform.scale.y = scaling.y;
			currentJoint->transform.scale.z = scaling.z;

			currentJoint->transform.rotation.x = rotation.x;
			currentJoint->transform.rotation.y = rotation.y;
			currentJoint->transform.rotation.z = rotation.z;
			currentJoint->transform.rotation.w = rotation.w;

			currentJoint->transform.translation.x = position.x;
			currentJoint->transform.translation.y = position.y;
			currentJoint->transform.translation.z = position.z;

			// resize space for children
			currentJoint->children.resize(inNode->mNumChildren);

			// loop all children
			for (unsigned int i = 0; i < inNode->mNumChildren; ++i)
			{
				// add the child into read list
				AddSkeletonData(inNode->mChildren[i], &currentJoint->children[i]);
			}
		}

		void AddAnimationChannelData(Mesh &outMesh, aiAnimation* inAnimation)
		{
			if(outMesh.skeleton == nullptr)
				return;

			if (inAnimation->mTicksPerSecond == 0) // 0 means undefined in file
				inAnimation->mTicksPerSecond = 24.0f; // 24fps is most common in 3D animation

			// Number of nodes
			unsigned int numberOfJoints = outMesh.skeleton->num_joints();
			// Basic info
			ozz::animation::offline::RawAnimation rawAnimation;
			rawAnimation.name = inAnimation->mName.C_Str();
			rawAnimation.duration = inAnimation->mDuration / inAnimation->mTicksPerSecond;
			rawAnimation.tracks.resize(numberOfJoints);

			// Channels
			for(unsigned int i=0; i<numberOfJoints; ++i)
			{

				// get ID of channel which contains the same joint name
				unsigned int channelID;
				if(!FindChanelID(outMesh.skeleton->joint_names()[i], inAnimation,channelID))
				{
					// if it cannot find the channel with the same name
					continue;
				}

				// translation
				for(int j=0; j < inAnimation->mChannels[channelID]->mNumPositionKeys; ++j)
				{
					ozz::animation::offline::RawAnimation::TranslationKey key = {
							(float)(inAnimation->mChannels[channelID]->mPositionKeys[j].mTime / inAnimation->mTicksPerSecond),
							ozz::math::Float3(
									inAnimation->mChannels[channelID]->mPositionKeys[j].mValue.x,
									inAnimation->mChannels[channelID]->mPositionKeys[j].mValue.y,
									inAnimation->mChannels[channelID]->mPositionKeys[j].mValue.z)};

					rawAnimation.tracks[i].translations.push_back(key);
				}

				// rotation
				for(int j=0; j < inAnimation->mChannels[channelID]->mNumRotationKeys; ++j)
				{

					ozz::animation::offline::RawAnimation::RotationKey key = {
							(float)(inAnimation->mChannels[channelID]->mRotationKeys[j].mTime / inAnimation->mTicksPerSecond),
							ozz::math::Quaternion(
									inAnimation->mChannels[channelID]->mRotationKeys[j].mValue.x,
									inAnimation->mChannels[channelID]->mRotationKeys[j].mValue.y,
									inAnimation->mChannels[channelID]->mRotationKeys[j].mValue.z,
									inAnimation->mChannels[channelID]->mRotationKeys[j].mValue.w)};

					rawAnimation.tracks[i].rotations.push_back(key);
				}

				// scale
				for(int j=0; j < inAnimation->mChannels[channelID]->mNumScalingKeys; ++j)
				{

					ozz::animation::offline::RawAnimation::ScaleKey key = {
							(float)(inAnimation->mChannels[channelID]->mScalingKeys[j].mTime / inAnimation->mTicksPerSecond),
							ozz::math::Float3(1.0f,1.0f,1.0f)};
					// bone scaling is rarely useful. And it makes the model too big.
//									inAnimation->mChannels[channelID]->mScalingKeys[j].mValue.x,
//									inAnimation->mChannels[channelID]->mScalingKeys[j].mValue.y,
//									inAnimation->mChannels[channelID]->mScalingKeys[j].mValue.z)};
					rawAnimation.tracks[i].scales.push_back(key);
				}
			}

			// turn data into runtime format
			ozz::animation::offline::AnimationBuilder animationBuilder;
			// Add new animation into animations list
			outMesh.animations.emplace_back();
			outMesh.animations.back() = animationBuilder(rawAnimation);
		}

		bool FindChanelID(const std::string& inBoneName, aiAnimation* inAnimation, unsigned int& trackID)
		{
			// if the number bigger than the biggest index, it has no channel
			trackID = 0;

			for(unsigned int i =0 ; i < inAnimation->mNumChannels; i++)
			{
				if(strcmp(inBoneName.c_str() , inAnimation->mChannels[i]->mNodeName.C_Str())==0)
				{
					trackID = i;
					return true;
				}
			}

			return false;
		}

		bool FindChanelID(const std::string& inBoneName, ozz::animation::Skeleton* inSkeleton, unsigned int& trackID)
		{
			// if the number bigger than the biggest index, it has no channel
			trackID = 0;

			if(inSkeleton == nullptr)
				return false;

			for(unsigned int i =0 ; i < inSkeleton->num_joints(); i++)
			{
				if(strcmp(inBoneName.c_str() , inSkeleton->joint_names()[i])==0)
				{
					trackID = i;
					return true;
				}
			}
			PLATINUM_WARNING("The bone name `"+ inBoneName +"` in FBX file doesn't match any track name.");
			return false;
		}

	}
}



