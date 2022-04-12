//
// Created by Matt & Jinyaun on 08/03/2022.
//

#include <OpenGL/Mesh.h>

#include <utility>

using namespace PlatinumEngine;

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices)
	:vertices(std::move(newVertices)), indices(std::move(newIndices))
{

}
Mesh::~Mesh()
{
}
