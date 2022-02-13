#include <OpenGL/ShaderInput.h>
// for catching errors
#include <OpenGL/GLCheck.h>

namespace PlatinumEngine
{
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
		SetVertices(vertices, verticesUsage);
		SetVertexAttributes<Vertex>({
				{ GL_FLOAT, 3, offsetof(Vertex, position) },
				{ GL_FLOAT, 3, offsetof(Vertex, normal) },
				{ GL_FLOAT, 2, offsetof(Vertex, textureCoordinate) }
		});
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
		GL_CHECK(glBindVertexArray(_vertexArrayObject));
		if (_elementBufferObject != 0)
			GL_CHECK(glDrawElements(drawingMode, _indicesSize, _indexType, 0));
		else if (_vertexBufferObject != 0)
			GL_CHECK(glDrawArrays(drawingMode, 0, _verticesSize));
		GL_CHECK(glBindVertexArray(0));
	}

	void ShaderInput::SetVertices(
			GLsizeiptr verticesSize,
			const void* vertices,
			std::type_index typeOfVertex,
			size_t sizeOfVertex,
			GLenum usage)
	{
		_verticesSize = verticesSize;
		_vertexType = typeOfVertex;

		if (_vertexBufferObject == 0)
			GL_CHECK(glGenBuffers(1, &_vertexBufferObject));

		// add VBO to VAO (If VAO == 0, VBO won't be added. This is intentional)
		GL_CHECK(glBindVertexArray(_vertexArrayObject));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeOfVertex, vertices, usage));
		GL_CHECK(glBindVertexArray(0));

		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void ShaderInput::SetVertexAttributes(
			size_t sizeOfVertex,
			const std::vector<VertexAttribute>& vertexAttributes)
	{
		if (_vertexArrayObject == 0)
			// generate if not generated yet
			GL_CHECK(glGenVertexArrays(1, &_vertexArrayObject));
		else
		{
			// cleanup old data
			for (GLuint i = 0; i < _vertexAttributesSize; ++i)
				GL_CHECK(glDisableVertexArrayAttrib(_vertexArrayObject, i));
		}

		// add VBO, EBO to VAO (If VBO == 0 || EBO == 0, VBO/EBO won't be added. This is intentional)
		GL_CHECK(glBindVertexArray(_vertexArrayObject));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject));
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject));

		_vertexAttributesSize = vertexAttributes.size();
		for (size_t i = 0; i < vertexAttributes.size(); ++i)
		{
			GL_CHECK(glVertexAttribPointer(
					i,
					vertexAttributes[i].numberOfComponents,
					vertexAttributes[i].elementType,
					GL_FALSE,
					(GLsizei)sizeOfVertex,
					(GLvoid*)vertexAttributes[i].offset));
			GL_CHECK(glEnableVertexAttribArray(i));
		}

		GL_CHECK(glBindVertexArray(0));
	}

	void ShaderInput::SetIndices(GLsizeiptr indicesSize, const void* indices, GLenum indexType, GLenum usage)
	{
		_indexType = indexType;
		_indicesSize = indicesSize;

		if (_elementBufferObject == 0)
			GL_CHECK(glGenBuffers(1, &_elementBufferObject));

		// add EBO to VAO (If VAO == 0, EBO won't be added. This is intentional)
		GL_CHECK(glBindVertexArray(_vertexArrayObject));
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexBufferObject));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * SizeOfGLenum(indexType), indices, usage));
		GL_CHECK(glBindVertexArray(0));

		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
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
		GL_CHECK(glDeleteBuffers(1, &_vertexBufferObject));
		// reset fields controlled by vertices data
		_vertexBufferObject = 0;
		_verticesSize = 0;
		_vertexAttributesSize = 0;
	}

	void ShaderInput::DeleteVertexAttributes()
	{
		GL_CHECK(glDeleteVertexArrays(1, &_vertexArrayObject));
		// reset fields controlled by vertex attributes data
		_vertexArrayObject = 0;
		_vertexType = std::type_index(typeid(void));
		_vertexAttributesSize = 0;
	}

	void ShaderInput::DeleteIndices()
	{
		GL_CHECK(glDeleteBuffers(1, &_elementBufferObject));
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
}