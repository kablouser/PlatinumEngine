#pragma once

/*
 * This is heavily based on SFML/src/SFML/Graphics/GLCheck.hpp
 */

namespace PlatinumEngine
{
	// Create macro for catching OpenGL error codes
#ifdef NDEBUG

	// In release mode, we don't check for error codes
	// Avoid overhead
#define GL_CHECK(expr) (expr)

#else

	// In debug mode...

	/**
	 * Put this macro every OpenGL function call. For example:
	 * GL_CHECK(glEnable(GL_DEPTH_TEST));
	 *
	 * This checks for OpenGL error codes before AND after the code statement.
	 * Any error codes found will be printed in std::cerr.
	 *
	 * The do-while loop is needed so that glCheck can be used as a single statement in if/else branches:
	 * if (true)
	 *     GL_CHECK(glEnable(GL_DEPTH_TEST));
	 */
#define GL_CHECK(expression) \
do  \
{   \
	PlatinumEngine::GLCheckError(__FILE__, __LINE__, #expression, false); \
	expression;                                                           \
	PlatinumEngine::GLCheckError(__FILE__, __LINE__, #expression, true);  \
} while (false)

#endif

	/**
	 * You shouldn't use this function directly. Use the macro GL_CHECK().
	 * Reads all error codes currently in OpenGL's error stack, and prints them to std::cerr.
	 * @param file name of source file that called this function
	 * @param line line number in source file that called this function
	 * @param expression piece of code that this check is for
	 * @param isAfterExpression is this check called after the expression?
	 */
	void GLCheckError(const char* file, unsigned int line, const char* expression, bool isAfterExpression);
}
