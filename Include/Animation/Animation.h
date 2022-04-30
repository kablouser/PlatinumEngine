//
// Created by Shihua on 24/04/2022.
//

#pragma once

#include <vector>
#include <assimp/scene.h>
namespace PlatinumEngine
{

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


	class Bone{
	public:

		// __(( PARAMETER ))__
		std::string boneName;
		unsigned int trackID;
		Maths::Mat4 offsetMatrix;

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
		Bone(): trackID(0), offsetMatrix(1.f)
		{}
	};

	class VertexBone{
	public:

		// __(( PARAMETER ))__
		std::vector<unsigned int> trackIDs;
		std::vector<float> weights;

		// __(( FUNCTION ))__
		void AddTrack(unsigned int inTrackID, float inWeight)
		{
			for(unsigned int i =0 ; i< 4; i++)
			{
				if(weights[i] == 0.f)
				{
					trackIDs[i] = inTrackID;
					weights[i] = inWeight;
				}
			}
		}

		void Clean()
		{
			trackIDs.clear();
			weights.clear();
		}

		// __(( CONSTRUCTOR ))__
		VertexBone()
		{
			trackIDs.resize(4, 0);
			weights.resize(4, 0.f);
		}

	};

}