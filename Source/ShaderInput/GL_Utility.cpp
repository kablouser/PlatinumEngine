#include <ShaderInput/GL_Utility.h>

template<>
GLenum PlatinumEngine::GetGLenum<GLbyte>()
{
	return GL_BYTE;
}

template<>
GLenum PlatinumEngine::GetGLenum<GLubyte>()
{
	return GL_UNSIGNED_BYTE;
}

template<>
GLenum PlatinumEngine::GetGLenum<GLshort>()
{
	return GL_SHORT;
}

template<>
GLenum PlatinumEngine::GetGLenum<GLushort>()
{
	return GL_UNSIGNED_SHORT;
}

template<>
GLenum PlatinumEngine::GetGLenum<GLint>()
{
	return GL_INT;
}

template<>
GLenum PlatinumEngine::GetGLenum<GLuint>()
{
	return GL_UNSIGNED_INT;
}

template<>
GLenum PlatinumEngine::GetGLenum<GLfloat>()
{
	return GL_FLOAT;
}

template<>
GLenum PlatinumEngine::GetGLenum<GLdouble>()
{
	return GL_DOUBLE;
}

size_t PlatinumEngine::SizeOfGLenum(GLenum glEnum)
{
	/* From https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribFormat.xhtml
	 *
	 * GL_BYTE, GL_SHORT, GL_INT, GL_FIXED, GL_FLOAT, GL_HALF_FLOAT, and GL_DOUBLE
	 * indicate types GLbyte, GLshort, GLint, GLfixed, GLfloat, GLhalf, and GLdouble, respectively;
	 *
	 *
	 * the values GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, and GL_UNSIGNED_INT
	 * indicate types GLubyte, GLushort, and GLuint, respectively;
	 *
	 *
	 * the values GL_INT_2_10_10_10_REV and GL_UNSIGNED_INT_2_10_10_10_REV
	 * indicating respectively four signed or unsigned elements packed into a single GLuint;
	 *
	 * and the value GL_UNSIGNED_INT_10F_11F_11F_REV indicating three floating point values packed into a single GLuint.
	 */
	switch (glEnum)
	{
	case GL_BYTE:
		return sizeof(GLbyte);
	case GL_SHORT:
		return sizeof(GLshort);
	case GL_INT:
		return sizeof(GLint);
	case GL_FIXED:
		return sizeof(GLfixed);
	case GL_FLOAT:
		return sizeof(GLfloat);
	case GL_HALF_FLOAT:
		return sizeof(GLhalf);
	case GL_DOUBLE:
		return sizeof(GLdouble);

	case GL_UNSIGNED_BYTE:
		return sizeof(GLubyte);
	case GL_UNSIGNED_SHORT:
		return sizeof(GLushort);
	case GL_UNSIGNED_INT:
		return sizeof(GLuint);

	case GL_INT_2_10_10_10_REV:
	case GL_UNSIGNED_INT_2_10_10_10_REV:
	case GL_UNSIGNED_INT_10F_11F_11F_REV:
		return sizeof(GLuint);

	default:
		return 0;
	}
}