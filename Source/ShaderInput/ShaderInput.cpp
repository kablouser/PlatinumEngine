#include <SFML/Graphics/GLCheck.hpp>
#include <ShaderInput/ShaderInput.h>

using namespace PlatinumEngine;

ShaderInput::ShaderInput() :
		_vertexBufferObject(0),
		_vertexArrayObject(0),
		_elementBufferObject(0),

		_vertexType(typeid(void)),
		_indexType(0),

		_verticesSize(0),
		_indicesSize(0),

		_vertexAttributesSize(0)
{
}

ShaderInput::ShaderInput(
		const std::vector<Vertex>& vertices,
		const std::vector<GLuint>& indices,
		GLenum verticesUsage,
		GLenum indicesUsage) :
		ShaderInput()
{
	VertexAttribute x = { GL_FLOAT, 3, offsetof(Vertex, position) };
	SetVertexAttributes<Vertex>({
			{ GL_FLOAT, 3, offsetof(Vertex, position) },
			{ GL_FLOAT, 3, offsetof(Vertex, normal) },
			{ GL_FLOAT, 2, offsetof(Vertex, textureCoordinate) }
	});
	SetVertices(vertices, verticesUsage);
	SetIndices(indices, indicesUsage);
}

ShaderInput::~ShaderInput()
{
	DeleteVertices();
	DeleteVertexAttributes();
	DeleteIndices();
}

//--------------------------------------------------------------------------------------------------------------
// Primary important functions.
//--------------------------------------------------------------------------------------------------------------

void ShaderInput::Draw(GLenum drawingMode) const
{
	glBindVertexArray(_vertexArrayObject);
	if (_elementBufferObject != 0)
		glDrawElements(drawingMode, _indicesSize, _indexType, 0);
	else if (_vertexBufferObject != 0)
		glDrawArrays(drawingMode, 0, _verticesSize);
	glBindVertexArray(0);
}

void ShaderInput::SetVertexAttributes(std::type_index typeOfVertex, size_t sizeOfVertex,
		const std::vector<VertexAttribute>& vertexAttributes)
{
	// store the data type in an unique format
	_vertexType = typeOfVertex;

	if (_vertexArrayObject == 0)
		// generate if not generated yet
		glGenVertexArrays(1, &_vertexArrayObject);
	else
	{
		// cleanup old data
		for (GLuint i = 0; i < _vertexAttributesSize; ++i)
			glDisableVertexArrayAttrib(_vertexArrayObject, i);
	}

	// add VBO, EBO to VAO (If VBO == 0 || EBO == 0, VBO/EBO won't be added. This is intentional)
	glBindVertexArray(_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);

	_vertexAttributesSize = vertexAttributes.size();
	for (size_t i = 0; i < vertexAttributes.size(); ++i)
	{
		glVertexAttribPointer(
				i,
				vertexAttributes[i].numberOfComponents,
				vertexAttributes[i].elementType,
				GL_FALSE,
				(GLsizei)sizeOfVertex,
				(GLvoid*)vertexAttributes[i].offset);
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}

void ShaderInput::SetVertices(GLsizeiptr verticesSize, const void* vertices, size_t sizeOfVertex, GLenum usage)
{
	_verticesSize = verticesSize;

	if (_vertexBufferObject == 0)
		glGenBuffers(1, &_vertexBufferObject);

	// add VBO to VAO (If VAO == 0, VBO won't be added. This is intentional)
	glBindVertexArray(_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeOfVertex, vertices, usage);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShaderInput::SetIndices(GLsizeiptr indicesSize, const void* indices, GLenum indexType, GLenum usage)
{
	_indexType = indexType;
	_indicesSize = indicesSize;

	if (_elementBufferObject == 0)
		glGenBuffers(1, &_elementBufferObject);

	// add EBO to VAO (If VAO == 0, EBO won't be added. This is intentional)
	glBindVertexArray(_vertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * SizeOfGLenum(indexType), indices, usage);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------------------------------------------------------
// Get sizes.
//--------------------------------------------------------------------------------------------------------------

GLsizeiptr ShaderInput::GetVerticesSize() const
{
	return _verticesSize;
}

GLuint ShaderInput::GetVertexAttributesSize() const
{
	return _vertexAttributesSize;
}

GLsizeiptr ShaderInput::GetIndicesSize() const
{
	return _indicesSize;
}

//--------------------------------------------------------------------------------------------------------------
// Has checks
//--------------------------------------------------------------------------------------------------------------

bool ShaderInput::HasVertices() const
{
	return _vertexBufferObject != 0;
}

bool ShaderInput::HasVertexAttributes() const
{
	return _vertexArrayObject != 0;
}

bool ShaderInput::HasIndices() const
{
	return _elementBufferObject != 0;
}

//--------------------------------------------------------------------------------------------------------------
// Deletes
// These delete functions only work when a valid OpenGL context exists.
// Hence, ShaderInput : sf::GlResource ensures that.
//--------------------------------------------------------------------------------------------------------------

void ShaderInput::DeleteVertices()
{
	glDeleteBuffers(1, &_vertexBufferObject);
	// reset fields controlled by vertices data
	_vertexBufferObject = 0;
	_verticesSize = 0;
	_vertexAttributesSize = 0;
}

void ShaderInput::DeleteVertexAttributes()
{
	glDeleteVertexArrays(1, &_vertexArrayObject);
	// reset fields controlled by vertex attributes data
	_vertexArrayObject = 0;
	_vertexType = std::type_index(typeid(void));
	_vertexAttributesSize = 0;
}

void ShaderInput::DeleteIndices()
{
	glDeleteBuffers(1, &_elementBufferObject);
	// reset fields controlled by indices data
	_elementBufferObject = 0;
	_indexType = 0;
	_indicesSize = 0;
}

//--------------------------------------------------------------------------------------------------------------
// Reading operations. For testing purposes.
//--------------------------------------------------------------------------------------------------------------

ShaderInput::ShaderInputState ShaderInput::ReadState() const
{
	return {
			_vertexBufferObject,
			_vertexArrayObject,
			_elementBufferObject,
			_vertexType,
			_indexType,
			_verticesSize, _indicesSize,
			_vertexAttributesSize
	};
}
