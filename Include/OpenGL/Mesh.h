//
// Created by Matt on 08/03/2022.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <OpenGL/Vertex.h>
#include <OpenGL/Texture.h>

namespace PlatinumEngine {
	/**
	 * Data structure to store mesh data
	 * Assumes triangle faces for now
	 */
	class Mesh {
	public:
		Mesh();
		Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
		std::vector<Vertex> GetVertices() const;
		void SetVertices(std::vector<Vertex> &vertices);
		std::vector<unsigned int> GetIndices() const;
		void SetIndices(std::vector<unsigned int> &indices);

	private:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
	};
}