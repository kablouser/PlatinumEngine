#pragma once

// glew.h replaces gl.h. So don't #include <SFML/OpenGL.hpp>
#include "GL/glew.h"

namespace PlatinumEngine
{
	/**
	 * Converts a OpenGL type into it's respective GLenum if possible.
	 * Except {GLfixed, GLhalf}, because it's the same as {GLint, GLushort} respectively
	 * as far as C++ templates are concerned.
	 * @tparam T Must be a OpenGL data type, see https://www.khronos.org/opengl/wiki/OpenGL_Type.
	 * @return GLenum representing the same type iff type has a GLenum, otherwise 0
	 */
	template<typename T>
	GLenum GetGLenum();

	/**
	 * @param glEnum must be one of {GL_BYTE, GL_SHORT, GL_INT, GL_FIXED, GL_FLOAT, GL_HALF_FLOAT, GL_DOUBLE,\n
	 * GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT,\n
	 * GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV
	 * @return if glEnum is valid returns number of bytes represented by that type, otherwise 0.
	 */
	size_t SizeOfGLenum(GLenum glEnum);
}
