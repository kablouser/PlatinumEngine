//
// Created by Shihua on 24/04/2022.
//

// PLATINUM Library
#include <Animation/Animation.h>

namespace PlatinumEngine
{

	void Bone::SetTranformMatrixByaiMatrix(const aiMatrix4x4& aiMatrix)
	{
		offsetMatrix = Maths::Mat4({
				aiMatrix[0][0], aiMatrix[1][0], aiMatrix[2][0], aiMatrix[3][0],
				aiMatrix[0][1], aiMatrix[1][1], aiMatrix[2][1], aiMatrix[3][1],
				aiMatrix[0][2], aiMatrix[1][2], aiMatrix[2][2], aiMatrix[3][2],
				aiMatrix[0][3], aiMatrix[1][3], aiMatrix[2][3], aiMatrix[3][3] });
	}

	Bone::Bone() : trackID(0), offsetMatrix(1.f){}
}
