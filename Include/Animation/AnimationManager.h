//
// Created by Shihua on 24/04/2022.
//

#pragma once

#include <OpenGL/Mesh.h>
#include <utility>
#include <Logger/Logger.h>

#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/animation/runtime/skeleton.h"
#include "ozz/base/log.h"
#include "ozz/base/io/stream.h"
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"
#include "ozz/options/options.h"

namespace PlatinumEngine
{
	class AnimationManager
	{
	public:
		// {  PARAMETER  }

		// {  FUNCTION_  }
		void SelectAnimation()
		{
			if(!_mesh->animations.empty())
				_selectedAnimation = &_mesh->animations[0];
		}
		// { CONSTRUCTOR }

	private:
		// {  PARAMETER  }
		Mesh* _mesh;
		Animation* _selectedAnimation = nullptr;

		// {  FUNCTION_  }
		bool FindChannel(const std::string& nodeName, unsigned int& channelIndex)
		{

			if(_selectedAnimation == nullptr)
				return false;


			channelIndex = 0;
			for(unsigned int i =0 ; !_selectedAnimation->channels.empty();i++)
			{
				if(_selectedAnimation->channels[i].nodeName == nodeName)
				{
					channelIndex = i;
					return true;
				}
			}

			return false;
		}

		bool FindTranslationFrame(float animationTime, unsigned int channelIndex, unsigned int& keyFrameIndex)
		{

			keyFrameIndex = 0;

			unsigned int channelSize=_selectedAnimation->channels[channelIndex].translationValue.size();

			if(channelSize == 1)
			{
				return true;
			}

			for(unsigned int i = 1; i < channelSize; ++i)
			{
				if(animationTime < _selectedAnimation->channels[channelIndex].translationValue[i].timePoint)
				{
					keyFrameIndex = i-1;

					return true;
				}
			}

			PLATINUM_WARNING(channelSize != 0 ? "The animation time is invalid.": "There is 0 translation key.");
			return false;
		}

		bool FindScaleFrame(float animationTime, unsigned int channelIndex, unsigned int& keyFrameIndex)
		{

			keyFrameIndex = 0;

			unsigned int channelSize=_selectedAnimation->channels[channelIndex].scaleValue.size();

			if(channelSize == 1)
			{
				return true;
			}

			for(unsigned int i = 1; i < channelSize; ++i)
			{
				if(animationTime < _selectedAnimation->channels[channelIndex].scaleValue[i].timePoint)
				{
					keyFrameIndex = i-1;

					return true;
				}
			}

			PLATINUM_WARNING(channelSize != 0 ? "The animation time is invalid.": "There is 0 scale key.");
			return false;
		}

		bool FindRotationFrame(float animationTime, unsigned int channelIndex, unsigned int& keyFrameIndex)
		{

			keyFrameIndex = 0;

			unsigned int channelSize=_selectedAnimation->channels[channelIndex].rotationValue.size();

			if(channelSize == 1)
			{
				return true;
			}

			for(unsigned int i = 1; i < channelSize; ++i)
			{
				if(animationTime < _selectedAnimation->channels[channelIndex].rotationValue[i].timePoint)
				{
					keyFrameIndex = i-1;

					return true;
				}
			}

			PLATINUM_WARNING(channelSize != 0 ? "The animation time is invalid.": "There is 0 rotation key.");
			return false;
		}


		void InterpolateTranslation(float animationTime, const std::string& nodeName, Maths::Vec3& outValue)
		{
			// find targeted channel
			unsigned int channelIndex;
			if(!FindChannel(nodeName,channelIndex))
			{
				return;
			}

			// find targeted frame for translation
			unsigned int startFrameIndex;
			if(!FindTranslationFrame(animationTime, channelIndex, startFrameIndex))
			{
				return;
			}

			// get the start transformation info for the start frame and end frame
			Vec3Key keyForStartFrame = _selectedAnimation->channels[channelIndex].translationValue[startFrameIndex];
			Vec3Key keyForEndFrame = _selectedAnimation->channels[channelIndex].translationValue[startFrameIndex+1];

			// interpolation
			float t = (animationTime - keyForStartFrame.timePoint)/ (keyForEndFrame.timePoint - keyForStartFrame.timePoint);

			outValue = (1-t) * keyForStartFrame.vec3 + t * keyForEndFrame.vec3;

		}

		void InterpolateScale(float animationTime, const std::string& nodeName, Maths::Vec3& outValue)
		{
			// find targeted channel
			unsigned int channelIndex;
			if(!FindChannel(nodeName,channelIndex))
			{
				return;
			}

			// find targeted frame for translation
			unsigned int startFrameIndex;
			if(!FindTranslationFrame(animationTime, channelIndex, startFrameIndex))
			{
				return;
			}

			// get the start transformation info for the start frame and end frame
			Vec3Key keyForStartFrame = _selectedAnimation->channels[channelIndex].scaleValue[startFrameIndex];
			Vec3Key keyForEndFrame = _selectedAnimation->channels[channelIndex].scaleValue[startFrameIndex+1];

			// interpolation
			float t = (animationTime - keyForStartFrame.timePoint)/ (keyForEndFrame.timePoint - keyForStartFrame.timePoint);

			outValue = (1-t) * keyForStartFrame.vec3 + t * keyForEndFrame.vec3;

		}

		void InterpolateQuaternion(float animationTime, const std::string& nodeName, Maths::Quaternion& outValue)
		{
			// find targeted channel
			unsigned int channelIndex;
			if(!FindChannel(nodeName,channelIndex))
			{
				return;
			}

			// find targeted frame for translation
			unsigned int startFrameIndex;
			if(!FindTranslationFrame(animationTime, channelIndex, startFrameIndex))
			{
				return;
			}

			// get the start transformation info for the start frame and end frame
			QuatKey keyForStartFrame = _selectedAnimation->channels[channelIndex].rotationValue[startFrameIndex];
			QuatKey keyForEndFrame = _selectedAnimation->channels[channelIndex].rotationValue[startFrameIndex+1];

			// interpolation
			float t = (animationTime - keyForStartFrame.timePoint)/ (keyForEndFrame.timePoint - keyForStartFrame.timePoint);

			outValue =  Maths::Quaternion::Slerp(keyForStartFrame.quat , keyForEndFrame.quat, t);
		}


		void ComputeHierarchyTranform(float animationTime, BoneNode* node, Maths::Mat4 &parentTransform)
		{
			// interpolate the animations
			Maths::Vec3 translationValue, scaleValue;
			Maths::Quaternion rotationValue;
			InterpolateTranslation(animationTime, node->name,translationValue);
			InterpolateScale(animationTime, node->name,scaleValue);
			InterpolateQuaternion(animationTime, node->name,rotationValue);

			// loop through children
			for(auto &child : node->children)
			{
				ComputeHierarchyTranform(animationTime, child, parentTransform);
			}
		}





		// { CONSTRUCTOR }
	};

}