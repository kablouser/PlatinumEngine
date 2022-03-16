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

	// utility uniform functions implemented here
	void ShaderProgram::SetUniform(const std::string& name, bool value) const
	{
		GL_CHECK(glUniform1i(glGetUniformLocation(_shaderProgramHandle, name.c_str()), (int)value));
	}

	void ShaderProgram::SetUniform(const std::string& name, int value) const
	{
		GL_CHECK(glUniform1i(glGetUniformLocation(_shaderProgramHandle, name.c_str()), value));
	}

	void ShaderProgram::SetUniform(const std::string &name, float value) const
	{
		GL_CHECK(glUniform1f(glGetUniformLocation(_shaderProgramHandle, name.c_str()), value));
	}


	void ShaderProgram::SetUniform(const std::string &name, float x, float y) const
	{
		GL_CHECK(glUniform2f(glGetUniformLocation(_shaderProgramHandle, name.c_str()), x, y));
	}

	void ShaderProgram::SetUniform(const std::string &name, float x, float y, float z) const
	{
		GL_CHECK(glUniform3f(glGetUniformLocation(_shaderProgramHandle, name.c_str()), x, y, z));
	}


	void ShaderProgram::SetUniform(const std::string &name, float x, float y, float z, float w) const
	{
		GL_CHECK(glUniform4f(glGetUniformLocation(_shaderProgramHandle, name.c_str()), x, y, z, w));
	}

	void ShaderProgram::SetUniform(const std::string &name, const Maths::Vec2 &value) const
	{
		GL_CHECK(glUniform2fv(glGetUniformLocation(_shaderProgramHandle, name.c_str()), 1, &value[0]));
	}

	void ShaderProgram::SetUniform(const std::string &name, const Maths::Vec3 &value) const
	{
		GL_CHECK(glUniform3fv(glGetUniformLocation(_shaderProgramHandle, name.c_str()), 1, &value[0]));
	}

	void ShaderProgram::SetUniform(const std::string &name, const Maths::Vec4 &value) const
	{
		GL_CHECK(glUniform4fv(glGetUniformLocation(_shaderProgramHandle, name.c_str()), 1, &value[0]));
	}

	void ShaderProgram::SetUniform(const std::string &name, const glm::mat2 &value) const
	{
		GL_CHECK(glUniformMatrix2fv(glGetUniformLocation(_shaderProgramHandle, name.c_str()), 1, GL_FALSE, &value[0][0]));
	}

	void ShaderProgram::SetUniform(const std::string &name, const glm::mat3 &value) const
	{
		GL_CHECK(glUniformMatrix3fv(glGetUniformLocation(_shaderProgramHandle, name.c_str()), 1, GL_FALSE, &value[0][0]));
	}

	void ShaderProgram::SetUniform(const std::string &name, const Maths::Mat4 &value) const
	{
		GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(_shaderProgramHandle, name.c_str()), 1, GL_FALSE, &value.matrix[0]));
	}

	void ShaderProgram::SetUniform(const std::string &name, const glm::mat4 &value) const
	{
		GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(_shaderProgramHandle, name.c_str()), 1, GL_FALSE, &value.matrix[0]));
	}
}