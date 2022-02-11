#pragma once

// glew.h replaces gl.h. So don't #include <SFML/OpenGL.hpp>
#include "GL/glew.h"

namespace PlatinumEngine
{
	/**
	 * Converts a OpenGL type into it's respective GLenum if possible.
	 * Except {GLfixed, GLhalf}, because it's the same as {GLint, GLushort } respectively as far as C++ templates are concerned.
	 * @tparam T Must be a OpenGL data type, see https://www.khronos.org/opengl/wiki/OpenGL_Type.
	 * @return GLenum representing the same type iff type has a GLenum, otherwise 0
	 */
	template<typename T>
	GLenum GetGLenum();

	size_t SizeOfGLenum(GLenum glEnum);
}
