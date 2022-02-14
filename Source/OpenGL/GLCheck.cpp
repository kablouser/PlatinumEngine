/*
 * This is heavily based on SFML/src/SFML/Graphics/GLCheck.cpp
 */

#include <OpenGL/GLCheck.h>
// glew.h replaces gl.h. So don't #include <SFML/OpenGL.hpp>
#include <GL/glew.h>

#include <string>
#include <iostream>

// for checking OpenGL context
#include <SFML/Window/Context.hpp>

namespace PlatinumEngine
{
	void GLCheckError(const char* file, unsigned int line, const char* expression, bool isAfterExpression)
	{
		// if there's no current context, glGetError produces infinite loop
		// (good job OpenGL, couldn't think of a special error code for this scenario)
		if(sf::Context::getActiveContext() == nullptr)
			return;

		GLenum errorCode;
		// How many errors are we up to currently?
		int errorCount = 0;

		// There could be multiple errors
		// Loop through errors stack
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			++errorCount;

			std::string fileString = file;
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

			if(isAfterExpression)
			{
				// expression caused the errors
				if (errorCount == 1)
				{
					// Only print the file and expression once
					std::cerr << "Internal OpenGL error(s) on "
							  << fileString.substr(fileString.find_last_of("\\/") + 1) << "(" << line << ")."
							  << std::endl
					          << "    " << expression << std::endl;
				}
				std::cerr << "    " << error << std::endl;
			}
			else
			{
				// expression did NOT cause the errors
				if (errorCount == 1)
				{
					// Only print the file once
					std::cerr << "Internal OpenGL error(s) was not caught until now "
							  << fileString.substr(fileString.find_last_of("\\/") + 1) << "(" << line << ")."
							  << std::endl;
				}
				std::cerr << "    " << error << std::endl;
			}
		}

		if(0 < errorCount)
		{
			// make multi-line output more tidy
			std::cerr << std::endl;
		}
	}
}