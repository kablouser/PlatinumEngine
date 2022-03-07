#include <OpenGL/ShaderProgram.h>
// for printing errors
#include <iostream>
#include <OpenGL/GLCheck.h>

namespace PlatinumEngine
{
	ShaderProgram::ShaderProgram() :
			_shaderProgramHandle(0)
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		GL_CHECK(glDeleteProgram(_shaderProgramHandle));
	}

	bool ShaderProgram::Compile(
			const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource)
	{
		GLuint vertexShader, fragmentShader;
		GL_CHECK(vertexShader = glCreateShader(GL_VERTEX_SHADER));
		GL_CHECK(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

		if (CompileShaderSource(vertexShader, vertexShaderSource.c_str(), "vertex") &&
			CompileShaderSource(fragmentShader, fragmentShaderSource.c_str(), "fragment"))
		{
			if (_shaderProgramHandle == 0)
				GL_CHECK(_shaderProgramHandle = glCreateProgram());

			GL_CHECK(glAttachShader(_shaderProgramHandle, vertexShader));
			GL_CHECK(glAttachShader(_shaderProgramHandle, fragmentShader));
			GL_CHECK(glLinkProgram(_shaderProgramHandle));

			int success;
			GL_CHECK(glGetProgramiv(_shaderProgramHandle, GL_LINK_STATUS, &success));
			if (!success)
			{
				char infoLog[512];
				GL_CHECK(glGetProgramInfoLog(_shaderProgramHandle, 512, NULL, infoLog));
				std::cerr << "Shader program link error\n" << infoLog << std::endl;
				return false;
			}

			return true;
		}

		GL_CHECK(glDeleteShader(vertexShader));
		GL_CHECK(glDeleteShader(fragmentShader));
		return false;
	}

	void ShaderProgram::Bind() const
	{
		GL_CHECK(glUseProgram(_shaderProgramHandle));
	}

	void ShaderProgram::Unbind() const
	{
		GL_CHECK(glUseProgram(0));
	}

	GLuint ShaderProgram::GetHandle() const
	{
		return _shaderProgramHandle;
	}

	bool ShaderProgram::CompileShaderSource(GLuint shader, const char* source, const char* shaderName)
	{
		GL_CHECK(glShaderSource(shader, 1, &source, NULL));
		GL_CHECK(glCompileShader(shader));

		int success;
		GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			char infoLog[512];
			GL_CHECK(glGetShaderInfoLog(shader, 512, nullptr, infoLog));
			std::cerr <<
					  "Shader \"" << shaderName << "\" compilation error" << std::endl <<
					  infoLog << std::endl;
			return false;
		}

		return true;
	}
}