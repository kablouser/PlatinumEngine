/*
 * This is heavily based on SFML/src/SFML/Graphics/GLCheck.cpp
 */

#include <OpenGL/GLCheck.h>
#include <Logger/Logger.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace PlatinumEngine
{
	void GLCheckError(const char* file, unsigned int line, const char* expression, bool isAfterExpression)
	{
		// if there's no OpenGL context, glGetError produces infinitely GL_INVALID_OPERATION
		// (good job OpenGL, couldn't think of a special error code for this scenario)
		if(glfwGetCurrentContext() == nullptr)
		{
			// only print once per macro
			if(!isAfterExpression)
				Logger::LogError("GL_CHECK used outside of a OpenGL context", file, line);
			return;
		}

		GLenum errorCode;

		// There could be multiple errors
		// Loop through errors stack
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error = "Unknown error";

			switch (errorCode)
			{
			case GL_INVALID_ENUM:
				error = "GL_INVALID_ENUM";
				break;

			case GL_INVALID_VALUE:
				error = "GL_INVALID_VALUE";
				break;

			case GL_INVALID_OPERATION:
				error = "GL_INVALID_OPERATION";
				break;

			case GL_STACK_OVERFLOW:
				error = "GL_STACK_OVERFLOW";
				break;

			case GL_STACK_UNDERFLOW:
				error = "GL_STACK_UNDERFLOW";
				break;

			case GL_OUT_OF_MEMORY:
				error = "GL_OUT_OF_MEMORY";
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				error = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			}

			std::string errorMessage;
			if(isAfterExpression)
				errorMessage = "OpenGL error. ";
			else
				errorMessage = "OpenGL error was not caught until now. ";

			Logger::LogError(errorMessage + error, file, line);
		}
	}
}