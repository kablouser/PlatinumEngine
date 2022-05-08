//
// Created by Shihua on 24/04/2022.
//

#pragma once
#define OZZ_BUILD_SIMD_REF
// Unclassified
#include <vector>
#include <assimp/scene.h>
#include <utility>

// PLATINUM Library
#include <Maths/Matrices.h>
#include <Logger/Logger.h>


//OZZ Library
//DATA STRUCTURE
#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/skeleton.h"
#include "ozz/animation/offline/raw_animation.h"
#include "ozz/animation/offline/raw_skeleton.h"
//JOB
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
//BUILDER
#include "ozz/animation/offline/skeleton_builder.h"
#include "ozz/animation/offline/animation_builder.h"

//OZZ Library
//MATH
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"

namespace PlatinumEngine
{

	class Bone{
	public:

		// __(( PARAMETER ))__
		std::string boneName;
		unsigned int trackID;
		Maths::Mat4 offsetMatrix;

		// __(( FUNCTION ))__
		void SetTranformMatrixByaiMatrix(const aiMatrix4x4& aiMatrix);

		// __(( CONSTRUCTOR ))__
		Bone();

	};

}