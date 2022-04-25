//
// Created by Shihua on 24/04/2022.
//

#pragma once

#include <vector>
#include <assimp/scene.h>
#include <Animation/BoneNode.h>
namespace PlatinumEngine
{
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
						Maths::Vec3{ channel->mPositionKeys[i].mValue.x, channel->mRotationKeys[i].mValue.y,
									 channel->mRotationKeys[i].mValue.z });
			}

			for(unsigned int i =0; i < channel->mNumScalingKeys; ++i)
			{
				nodeChannel.scaleValue.emplace_back(
						Maths::Vec3{ channel->mScalingKeys[i].mValue.x, channel->mScalingKeys[i].mValue.y,
									 channel->mScalingKeys[i].mValue.z });
			}

			for(unsigned int i =0; i < channel->mNumRotationKeys; ++i)
			{
				nodeChannel.rotationValue.emplace_back(
						Maths::Quaternion{ channel->mRotationKeys->mValue.w, channel->mRotationKeys->mValue.x, channel->mRotationKeys->mValue.y, channel->mRotationKeys->mValue.z });
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