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
	_dirty = true;
}

std::vector<unsigned int> Mesh::GetIndices() const {
	return _indices;
}

void Mesh::SetIndices(std::vector<unsigned int> &indices) {
	_indices = indices;
	_dirty = true;
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
	GL_CHECK(glBindVertexArray(_vertexArrayObject));
	glDrawElements(GL_TRIANGLES, _drawLength, GL_UNSIGNED_INT, nullptr);
	GL_CHECK(glBindVertexArray(0));

}
// Private Functions
void Mesh::Create()
{
	if(!glGenVertexArrays) return;

	GL_CHECK(glGenVertexArrays(1, &_vertexArrayObject));
	GL_CHECK(glGenBuffers(1, &_vertexBufferObject));
	GL_CHECK(glGenBuffers(1, &_elementBufferObject));

	GL_CHECK(glBindVertexArray(_vertexArrayObject));

	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject));

	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));
	GL_CHECK(glEnableVertexAttribArray(0));

	GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)));
	GL_CHECK(glEnableVertexAttribArray(1));

	GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoords)));
	GL_CHECK(glEnableVertexAttribArray(2));

	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject));

	GL_CHECK(glBindVertexArray(0));
}

void Mesh::Destroy()
{
	if(!glDeleteBuffers) return;

	GL_CHECK(glDeleteBuffers(1, &_elementBufferObject));
	GL_CHECK(glDeleteBuffers(1, &_vertexBufferObject));
	GL_CHECK(glDeleteVertexArrays(1, &_vertexArrayObject));
	_elementBufferObject = _vertexArrayObject = _vertexBufferObject = 0;
}
void Mesh::Update()
{
	GL_CHECK(glBindVertexArray(_vertexArrayObject));

	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_DYNAMIC_DRAW));

	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), _indices.data(), GL_DYNAMIC_DRAW));

	GL_CHECK(glBindVertexArray(0));

	_dirty = false;
}