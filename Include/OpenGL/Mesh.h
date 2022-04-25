//
// Created by Matt on 08/03/2022.
//

#pragma once

#include <vector>
#include <memory>
#include <OpenGL/Vertex.h>
#include <OpenGL/Texture.h>
#include <OpenGL/GLCheck.h>
#include <assimp/scene.h>
#include <Animation/BoneNode.h>
#include <Animation/Animation.h>

namespace PlatinumEngine {
	/**
	 * Data structure to store mesh data
	 * Assumes triangle faces for now
	 */
	class Mesh {
	public:

		// For vertices
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::string fileName;

		// For animation
		std::vector<Animation> animations;
		std::vector<BoneNode> nodes;
		std::vector<Bone> bones;

		// Record offset
		std::vector<unsigned int> offset;

		Mesh();
		Mesh(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices);
		~Mesh();
	};
}