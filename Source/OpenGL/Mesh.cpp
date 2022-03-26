//
// Created by Matt & Jinyaun on 08/03/2022.
//

#include <OpenGL/Mesh.h>

using namespace PlatinumEngine;

Mesh::Mesh()
{
	Create();
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
	Create();
	Recreate(vertices, indices);
}
Mesh::~Mesh()
{
	Destroy();
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

void Mesh::Recreate(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	_dirty = true;
	_vertices = vertices;
	_indices = indices;

	_drawLength = (GLuint)_indices.size();
}

void Mesh::Render()
{
	if(_dirty) Update();
	glBindVertexArray(_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, _drawLength, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}
// Private Functions
void Mesh::Create()
{
	if(!glGenVertexArrays) return;

	glGenVertexArrays(1, &_vertexArrayObject);
	glGenBuffers(1, &_vertexBufferObject);
	glGenBuffers(1, &_elementBufferObject);

	glBindVertexArray(_vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoords));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);

	glBindVertexArray(0);
}

void Mesh::Destroy()
{
	if(!glDeleteBuffers) return;

	glDeleteBuffers(1, &_elementBufferObject);
	glDeleteBuffers(1, &_vertexBufferObject);
	glDeleteVertexArrays(1, &_vertexArrayObject);
	_elementBufferObject = _vertexArrayObject = _vertexBufferObject = 0;
}
void Mesh::Update() {
	glBindVertexArray(_vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), _indices.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

	_dirty = false;
}