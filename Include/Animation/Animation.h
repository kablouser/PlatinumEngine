//
// Created by Shihua on 24/04/2022.
//

#pragma once

#include <vector>
#include <assimp/scene.h>
namespace PlatinumEngine
{

	class BoneNode
	{
	public:
		// __(( PARAMETER ))__

		// Node name
		std::string name;

		// Transformation matrix
		Maths::Mat4 transformMatrix;

		// mesh id for this node
		std::vector<unsigned int> meshIDs;

		// Parent & Children
		BoneNode* parent = nullptr;
		std::vector<BoneNode*> children;

		// Indices of vertices related to this bone
		std::vector<int> verticesIndices;


		// __(( FUNCTION ))__
		void SetTranformMatrixByaiMatrix(aiMatrix4x4 aiMatrix)
		{
			transformMatrix = Maths::Mat4({
					aiMatrix[0][0],aiMatrix[1][0],aiMatrix[2][0],aiMatrix[3][0],
					aiMatrix[0][1],aiMatrix[1][1],aiMatrix[2][1],aiMatrix[3][1],
					aiMatrix[0][2],aiMatrix[1][2],aiMatrix[2][2],aiMatrix[3][2],
					aiMatrix[0][3],aiMatrix[1][3],aiMatrix[2][3],aiMatrix[3][3]});
		}

		// __(( CONSTRUCTOR ))__
		BoneNode(){};
		BoneNode(std::string inName):name(inName){};


	private:
		// (( PARAMETER ))


		// (( FUNCTION ))


		// (( CONSTRUCTOR ))

	};

	class Vec3Key
	{
	public:
		float timePoint; // time duration when this vec3 value will stay
		Maths::Vec3 vec3;

		Vec3Key(float inTimePoint, Maths::Vec3 inVec3):timePoint(inTimePoint), vec3(inVec3){}
	};

	class QuatKey
	{
	public:
		float timePoint; // time duration when this quaternion value will stay
		Maths::Quaternion quat;

		QuatKey(float inTimePoint, Maths::Quaternion inQuat):timePoint(inTimePoint), quat(inQuat){};
	};

	class NodeChannel
	{
	public:
		// __(( ENUM ))__
		enum Behavior{Defalt, Constant, Linear, Repeat};

		// __(( PARAMETER ))__

		// Node name
		std::string nodeName;

		// Value for Transform matrix (these vectors contain matrices for all the frames)
		std::vector<Vec3Key> translationValue, scaleValue;
		std::vector<QuatKey> rotationValue;

		// Behavior type for the animation
		Behavior behaviorTypeForBeginning = Defalt;
		Behavior behaviorTypeForEnding = Defalt;

		// __(( FUNCTION ))__

		// __(( CONSTRUCTOR ))__

	};


	class Weight
	{
	public:
		// __(( PARAMETER ))__
		unsigned int vertexID;
		float weight;
	};


	class Bone{
	public:

		// __(( PARAMETER ))__
		std::string boneName;
		unsigned int boneID;
		Maths::Mat4 offsetMatrix;
		Maths::Mat4 finalTransformationMatrix;
		//std::vector<Weight> weights;

		// __(( FUNCTION ))__
		void SetTranformMatrixByaiMatrix(aiMatrix4x4 aiMatrix)
		{
			offsetMatrix = Maths::Mat4({
					aiMatrix[0][0],aiMatrix[1][0],aiMatrix[2][0],aiMatrix[3][0],
					aiMatrix[0][1],aiMatrix[1][1],aiMatrix[2][1],aiMatrix[3][1],
					aiMatrix[0][2],aiMatrix[1][2],aiMatrix[2][2],aiMatrix[3][2],
					aiMatrix[0][3],aiMatrix[1][3],aiMatrix[2][3],aiMatrix[3][3]});
		}

		// __(( CONSTRUCTOR ))__
		Bone(): boneID(0), finalTransformationMatrix(1.f), offsetMatrix(1.f)
		{}
	};

	class VertexBone{
	public:

		// __(( PARAMETER ))__
		std::vector<unsigned int> boneIDs;
		std::vector<float> weights;

		// __(( FUNCTION ))__
		void AddBone(unsigned int inBoneID, float inWeight)
		{
			boneIDs.emplace_back(inBoneID);
			weights.emplace_back(inWeight);
		}

		void Clean()
		{
			boneIDs.clear();
			weights.clear();
		}

		// __(( CONSTRUCTOR ))__

	};

	class Animation
	{

	public:
		// (( PARAMETER ))
		std::string animationName;
		double duration = -1.0;
		double tickPerSecond = 0.0;
		std::vector<NodeChannel> channels;

		// (( FUNCTION ))
		void AddChannelByaiNodeAnim(aiNodeAnim* channel)
		{
			NodeChannel nodeChannel;

			nodeChannel.nodeName = channel->mNodeName.C_Str();

			for(unsigned int i =0; i < channel->mNumPositionKeys; ++i)
			{
				nodeChannel.translationValue.emplace_back(
						Vec3Key{(float)channel->mPositionKeys[i].mTime, Maths::Vec3{ channel->mPositionKeys[i].mValue.x, channel->mRotationKeys[i].mValue.y,
									 channel->mRotationKeys[i].mValue.z }});
			}

			for(unsigned int i =0; i < channel->mNumScalingKeys; ++i)
			{
				nodeChannel.scaleValue.emplace_back(
						Vec3Key{(float)channel->mScalingKeys[i].mTime, Maths::Vec3{ channel->mScalingKeys[i].mValue.x, channel->mScalingKeys[i].mValue.y,
									   channel->mScalingKeys[i].mValue.z }});
			}

			for(unsigned int i =0; i < channel->mNumRotationKeys; ++i)
			{
				nodeChannel.rotationValue.emplace_back(
						QuatKey{(float)channel->mRotationKeys[i].mTime, Maths::Quaternion{ channel->mRotationKeys->mValue.w, channel->mRotationKeys->mValue.x,
											 channel->mRotationKeys->mValue.y, channel->mRotationKeys->mValue.z }});
			}

			// set mode for the beginning of the beginning key frame
			if(channel->mPreState == aiAnimBehaviour::aiAnimBehaviour_CONSTANT)
				nodeChannel.behaviorTypeForBeginning = NodeChannel::Constant;

			else if(channel->mPreState == aiAnimBehaviour::aiAnimBehaviour_DEFAULT)
				nodeChannel.behaviorTypeForBeginning = NodeChannel::Defalt;

			else if(channel->mPreState == aiAnimBehaviour::aiAnimBehaviour_LINEAR)
				nodeChannel.behaviorTypeForBeginning = NodeChannel::Linear;

			else if(channel->mPreState == aiAnimBehaviour::aiAnimBehaviour_REPEAT)
				nodeChannel.behaviorTypeForBeginning = NodeChannel::Repeat;


			// set mode for the beginning of the ending key frame
			if(channel->mPostState == aiAnimBehaviour::aiAnimBehaviour_CONSTANT)
				nodeChannel.behaviorTypeForEnding = NodeChannel::Constant;

			else if(channel->mPostState == aiAnimBehaviour::aiAnimBehaviour_DEFAULT)
				nodeChannel.behaviorTypeForEnding = NodeChannel::Defalt;

			else if(channel->mPostState == aiAnimBehaviour::aiAnimBehaviour_LINEAR)
				nodeChannel.behaviorTypeForEnding = NodeChannel::Linear;

			else if(channel->mPostState == aiAnimBehaviour::aiAnimBehaviour_REPEAT)
				nodeChannel.behaviorTypeForEnding = NodeChannel::Repeat;

			// add node channel
			channels.emplace_back(nodeChannel);
		}

		// (( CONSTRUCTOR ))


	private:
		// (( PARAMETER ))


		// (( FUNCTION ))


		// (( CONSTRUCTOR ))

	};
}