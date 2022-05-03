//
// Created by Matt on 08/03/2022.
//

#pragma once

#include <vector>
#include <memory>
#include <map>
#include <OpenGL/Vertex.h>
#include <OpenGL/Texture.h>
#include <OpenGL/GLCheck.h>
#include <assimp/scene.h>
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
		std::vector<Animation*> animations;
		std::vector<AnimationVertex> animationVertices;
		ozz::animation::offline::RawSkeleton rawSkeleton;
		ozz::unique_ptr<ozz::animation::Skeleton> skeleton;
		// Storing connection between vertices and bones/nodes
		std::vector<Bone> bones;
		// Mapping bones' name and id
		std::map<std::string, unsigned int> boneMapping;

		Mesh();
		Mesh(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices);
		~Mesh();

		Mesh(Mesh&&) noexcept = default;
		Mesh& operator=(Mesh&&) noexcept = default;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
	};
}