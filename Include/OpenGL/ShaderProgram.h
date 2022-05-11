#pragma once
// OpenGL data types
#include <GL/glew.h>
// for handling glsl shader source code
#include <string>
// gui
#include <imgui.h>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// PlatinumEngine math library
#include <Maths/Vectors.h>
#include <Maths/Matrices.h>

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

		// utility uniform functions
		void SetUniform(const std::string &name, bool value) const;

		void SetUniform(const std::string &name, int value) const;

		void SetUniform(const std::string &name, float value) const;

		void SetUniform(const std::string &name, float x, float y) const;

		void SetUniform(const std::string &name, float x, float y, float z) const;

		void SetUniform(const std::string &name, float x, float y, float z, float w) const;

		void SetUniform(const std::string &name, const Maths::Vec2 &value) const;

		void SetUniform(const std::string &name, const Maths::Vec3 &value) const;

		void SetUniform(const std::string &name, const Maths::Vec4 &value) const;

		void SetUniform(const std::string &name, const glm::mat2 &value) const;

		void SetUniform(const std::string &name, const glm::mat3 &value) const;

		void SetUniform(const std::string &name, const Maths::Mat3 &value) const;
		void SetUniform(const std::string &name, const Maths::Mat4 &value) const;

		void SetUniform(const std::string &name, const glm::mat4 &value) const;


	private:

		bool CompileShaderSource(GLuint shader, const char* source, const char* shaderName);

		GLuint _shaderProgramHandle;
	};
}
