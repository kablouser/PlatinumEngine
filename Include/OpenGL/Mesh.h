//
// Created by Matt on 08/03/2022.
//

#pragma once

#include <vector>
#include <OpenGL/Vertex.h>
#include <OpenGL/Texture.h>
#include <OpenGL/GLCheck.h>

namespace PlatinumEngine {
	/**
	 * Data structure to store mesh data
	 * Assumes triangle faces for now
	 */
	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Mesh();
		Mesh(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices);
		~Mesh();
	};
}