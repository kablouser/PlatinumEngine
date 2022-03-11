//
// Created by Matt & Jinyaun on 08/03/2022.
//

#include <OpenGL/Mesh.h>

using namespace PlatinumEngine;

Mesh::Mesh() {
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
	_vertices = vertices;
	_indices = indices;
}

std::vector<Vertex> Mesh::GetVertices() const {
	return _vertices;
}

void Mesh::SetVertices(std::vector<Vertex> &vertices) {
	_vertices = vertices;
}

std::vector<unsigned int> Mesh::GetIndices() const {
	return _indices;
}

void Mesh::SetIndices(std::vector<unsigned int> &indices) {
	_indices = indices;
}