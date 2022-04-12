#pragma once

// OpenGL types
#include <GL/glew.h>
// For supporting ImGui types
#include <imgui.h>
#include <string>
#include <stb/stb_image.h>
namespace PlatinumEngine
{
	class Texture
	{
	public:

		Texture();

		~Texture();

		void Create(GLsizei width, GLsizei height, const void* pixelData = nullptr);
		void Create(const std::string& filePath);

		GLuint GetOpenGLHandle() const;
		ImTextureID GetImGuiHandle() const;
		void Bind() const;
		void Unbind() const;

	private:

		GLuint _textureHandle;
	};
}