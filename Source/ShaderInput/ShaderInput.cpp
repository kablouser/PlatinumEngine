#include <ShaderInput/ShaderInput.h>

using namespace PlatinumEngine;

ShaderInput::ShaderInput() :
		_vertexBufferObject(0),
		_vertexArrayObject(0),
		_elementBufferObject(0),

		_verticesSize(0),
		_indicesSize(0),

		_verticesUsage(Usage::Static),
		_indicesUsage(Usage::Static)
{
}

ShaderInput::ShaderInput(const ShaderInput& copy)
{
	DeleteVertices();
	DeleteIndices();
}

ShaderInput::ShaderInput(ShaderInput&& moveFrom) noexcept
{
	DeleteVertices();
	DeleteIndices();
}

ShaderInput::~ShaderInput()
{
	DeleteVertices();
	DeleteIndices();
}

template<typename T>
bool ShaderInput::SetVertices(const std::vector<T>& vertices, ShaderInput::Usage usage)
{
	return 0;
}

template<typename T>
bool ShaderInput::SetIndices(const std::vector<T>& indices, ShaderInput::Usage usage)
{
	return 0;
}

bool ShaderInput::DeleteVertices()
{
	return false;
}

bool ShaderInput::DeleteIndices()
{
	return false;
}

std::size_t ShaderInput::GetVerticesSize() const
{
	return 0;
}

std::size_t ShaderInput::GetVIndicesSize() const
{
	return 0;
}

void ShaderInput::Bind() const
{

}

void ShaderInput::Draw() const
{

}

template<> GLenum ShaderInput::GetGLenum<GLbyte>()
{
	return GL_BYTE;
}

template<> GLenum ShaderInput::GetGLenum<GLubyte>()
{
	return GL_UNSIGNED_BYTE;
}

template<> GLenum ShaderInput::GetGLenum<GLshort>()
{
	return GL_SHORT;
}

template<> GLenum ShaderInput::GetGLenum<GLushort>()
{
	return GL_UNSIGNED_SHORT;
}

template<> GLenum ShaderInput::GetGLenum<GLint>()
{
	return GL_INT;
}

template<> GLenum ShaderInput::GetGLenum<GLuint>()
{
	return GL_UNSIGNED_INT;
}

template<> GLenum ShaderInput::GetGLenum<GLfloat>()
{
	return GL_FLOAT;
}

template<> GLenum ShaderInput::GetGLenum<GLdouble>()
{
	return GL_DOUBLE;
}
