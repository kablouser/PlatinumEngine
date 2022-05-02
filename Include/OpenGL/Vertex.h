//
// Created by Matt & Jinyuan on 08/03/2022.
//

#pragma once

#include <Maths/Vectors.h>
#include <Maths/Matrices.h>

namespace PlatinumEngine {
	struct Vertex {
		Maths::Vec3 position;
		Maths::Vec3 normal;
		Maths::Vec2 textureCoords;
		Maths::Vec3 tangent;
		Maths::Vec3 biTangent;
	};


	struct AnimationVertex {
		Maths::Vec3 position;
		Maths::Vec3 normal;
		Maths::Vec2 textureCoords;
		Maths::Vec3 tangent;
		Maths::Vec3 biTangent;

		// for animation
		unsigned int trackIDs[4] = {0};
		float weights[4] = {0};

		void AddTrack(unsigned int inTrackID, float inWeight)
		{
			for(int i =0 ; i< 4; i++)
			{
				if(weights[i] == 0.f)
				{
					trackIDs[i] = inTrackID;
					weights[i] = inWeight;
					break;
				}
			}
		}
	};
}

