//
// Created by Matt on 08/03/2022.
//

#pragma once

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
		~Mesh();
		std::vector<Vertex> GetVertices() const;
		void SetVertices(std::vector<Vertex> &vertices);
		std::vector<unsigned int> GetIndices() const;
		void SetIndices(std::vector<unsigned int> &indices);
		void Recreate(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
		void Render();

	private:
		bool _dirty;
		unsigned int _vertexArrayObject, _vertexBufferObject, _elementBufferObject;
		GLsizeiptr _drawLength;
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;

		void Create();
		void Destroy();
		void Update();

	};
}