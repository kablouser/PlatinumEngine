//
// Created by Matt & Jinyuan on 08/03/2022.
//

#pragma once

#include <Maths/Vectors.h>

namespace PlatinumEngine {
	struct Vertex {
		Maths::Vec3 position;
		Maths::Vec3 normal;
		Maths::Vec2 textureCoords;
		Maths::Vec3 tangent;
		Maths::Vec3 biTangent;
	};
}

