#pragma once

// OpenGL data types
#include <GL/glew.h>
// for handling glsl shader source code
#include <string>

namespace PlatinumEngine
{
	class ShaderProgram
	{
	public:

		ShaderProgram();

		~ShaderProgram();

		bool Compile(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		void Bind() const;
		void Unbind() const;

		GLuint GetHandle() const;

	private:

		bool CompileShaderSource(GLuint shader, const char* source, const char* shaderName);

		GLuint _shaderProgramHandle;
	};
}
