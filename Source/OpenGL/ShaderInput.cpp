#include <OpenGL/ShaderInput.h>
// for catching errors
#include <OpenGL/GLCheck.h>

namespace PlatinumEngine
{
	//------------------------------------------------------------------------------------------------------------------
	// Constructors/Destructors
	//------------------------------------------------------------------------------------------------------------------

	ShaderInput::ShaderInput() :
			_vertexArrayObject(0),
			_vertexBufferObject(0),
			_elementBufferObject(0),

			_typeOfIndex(0),
			_vertexAttributesLength(0),
			_drawLength(0)
	{
	}

	ShaderInput::ShaderInput(
			const std::vector<Vertex>& vertices,
			const std::vector<GLuint>& indices)
			: ShaderInput()
	{
		Set(vertices, indices);
	}

	ShaderInput::ShaderInput(ShaderInput&& other) noexcept
	{
		*this = std::move(other);
	}

	ShaderInput& ShaderInput::operator=(ShaderInput&& other) noexcept
	{
		if (this != &other)
		{
			Clear();

			_vertexArrayObject = other._vertexArrayObject;
			_vertexBufferObject = other._vertexBufferObject;
			_elementBufferObject = other._elementBufferObject;
			_typeOfIndex = other._typeOfIndex;
			_vertexAttributesLength = other._vertexAttributesLength;
			_drawLength = other._drawLength;

			other._vertexArrayObject = 0;
			other._vertexBufferObject = 0;
			other._elementBufferObject = 0;
			other._typeOfIndex = 0;
			other._vertexAttributesLength = 0;
			other._drawLength = 0;
		}
		return *this;
	}

	ShaderInput::~ShaderInput()
	{
		Clear();
	}

	//------------------------------------------------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------------------------------------------------

	void ShaderInput::Set(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
	{
		SetGeneric<Vertex, GLuint>({
						{ GL_FLOAT, 3, offsetof(Vertex, position) },
						{ GL_FLOAT, 3, offsetof(Vertex, normal) },
						{ GL_FLOAT, 2, offsetof(Vertex, textureCoords) },
						{ GL_FLOAT, 3, offsetof(Vertex, tangent) },
						{ GL_FLOAT, 3, offsetof(Vertex, biTangent) }},
				vertices, indices);
	}

	void ShaderInput::Set(const std::vector<AnimationVertex>& vertices, const std::vector<GLuint>& indices)
	{
		SetGeneric<AnimationVertex, GLuint>({
						{ GL_FLOAT, 3, offsetof(AnimationVertex, position)},
						{ GL_FLOAT, 3, offsetof(AnimationVertex, normal) },
						{ GL_FLOAT, 2, offsetof(AnimationVertex, textureCoords) },
						{ GL_FLOAT, 4, offsetof(AnimationVertex, trackIDs) },
						{ GL_FLOAT, 4, offsetof(AnimationVertex, weights) }
						},
				vertices, indices);
	}

	void ShaderInput::Clear()
	{
		GL_CHECK(glDeleteVertexArrays(1, &_vertexArrayObject));
		GL_CHECK(glDeleteBuffers(1, &_vertexBufferObject));
		GL_CHECK(glDeleteBuffers(1, &_elementBufferObject));

		_vertexArrayObject =
		_vertexBufferObject =
		_elementBufferObject =
		_typeOfIndex =
		_vertexAttributesLength =
		_drawLength = 0;
	}

	void ShaderInput::Draw(GLenum drawingMode) const
	{
		if(_drawLength == 0)
			return;
		GL_CHECK(glBindVertexArray(_vertexArrayObject));
		GL_CHECK(glDrawElements(drawingMode, _drawLength, _typeOfIndex, 0));
		// Alternative direct drawing without indexing
		// GL_CHECK(glDrawArrays(drawingMode, 0, _verticesSize));
		GL_CHECK(glBindVertexArray(0));
	}

	bool ShaderInput::IsCreated() const
	{
		return _vertexBufferObject != 0;
	}

	void ShaderInput::Set(
			const std::vector<VertexAttribute>& vertexAttributes,

			// vertices info for vbo
			size_t sizeOfVertex,
			size_t verticesLength,
			const void* vertices,


			GLenum typeOfIndex,

			// indices info for ebo
			size_t sizeOfIndex,
			size_t indicesLength,
			const void* indices,

			GLenum usage)
	{
		if (_vertexArrayObject == 0)
			GL_CHECK(glGenVertexArrays(1, &_vertexArrayObject));
		else
		{
			// cleanup old state data
			for (GLuint i = 0; i < _vertexAttributesLength; ++i)
				GL_CHECK(glDisableVertexArrayAttrib(_vertexArrayObject, i));
		}

		if (_vertexBufferObject == 0)
			GL_CHECK(glGenBuffers(1, &_vertexBufferObject));
		if (_elementBufferObject == 0)
			GL_CHECK(glGenBuffers(1, &_elementBufferObject));

		// Bind VAO
		GL_CHECK(glBindVertexArray(_vertexArrayObject));
		// Bind VBO and allocate vertices
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeOfVertex * verticesLength, vertices, usage));
		// Bind EBO and allocate indices
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndex * indicesLength, indices, usage));


		// vertices array attributes (info that are not belong to vbo or ebo)
		_vertexAttributesLength = vertexAttributes.size();
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

		// undo global state changes
		GL_CHECK(glBindVertexArray(0));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		_typeOfIndex = typeOfIndex;
		_drawLength = indicesLength;
	}
}