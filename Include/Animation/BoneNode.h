//
// Created by Shihua on 24/04/2022.
//
#pragma once
#include <string>
#include <vector>
#include <Maths/Matrices.h>
#include <Maths/Vectors.h>
#include <Maths/Quaternion.h>

namespace PlatinumEngine
{
	class BoneNode
	{

	public:
		// (( ENUM ))
		enum Behavior{Defalt, Constant, Linear, Repeat};

		// (( PARAMETER ))

		// Node name
		std::string name;

		// Offset transformation matrix
		Maths::Mat4 transformMatrix;

		// mesh id for this node
		std::vector<unsigned int> meshIDs;

		// Parent & Children
		BoneNode* parent = nullptr;
		std::vector<BoneNode*> children;

		// Indices of vertices related to this bone
		std::vector<int> verticesIndices;


		// (( FUNCTION ))
		void SetTranformMatrixByaiMatrix(aiMatrix4x4 aiMatrix)
		{
			transformMatrix = Maths::Mat4({
					aiMatrix[0][0],aiMatrix[1][0],aiMatrix[2][0],aiMatrix[3][0],
					aiMatrix[0][1],aiMatrix[1][1],aiMatrix[2][1],aiMatrix[3][1],
					aiMatrix[0][2],aiMatrix[1][2],aiMatrix[2][2],aiMatrix[3][2],
					aiMatrix[0][3],aiMatrix[1][3],aiMatrix[2][3],aiMatrix[3][3]});
		}

		// (( CONSTRUCTOR ))
		BoneNode(){};
		BoneNode(std::string inName):name(inName){};


	private:
		// (( PARAMETER ))


		// (( FUNCTION ))


		// (( CONSTRUCTOR ))

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
		std::vector<Maths::Vec3> translationValue, scaleValue;
		std::vector<Maths::Quaternion> rotationValue;

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
		std::vector<Weight> weights;
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

	};
}