#pragma once

// OpenGL types
#include <GL/glew.h>
// For supporting ImGui types
#include <imgui.h>
#include <string>
#include <vector>

namespace PlatinumEngine
{
	class Texture
	{
	public:

		Texture();

		~Texture();

		void Create(GLsizei width, GLsizei height, const void* pixelData = nullptr);

		void CreateCubeMap(std::vector<std::string> faces);

		GLuint GetOpenGLHandle() const;
		ImTextureID GetImGuiHandle() const;
		void Bind() const;
		void Unbind() const;

		void BindCubeMap() const;
		void UnbindCubeMap() const;

		std::string fileName;
	private:

		GLuint _textureHandle;
	};

	class PixelData
	{
	public:
		PixelData();
		~PixelData();
		void Create(const std::string& filePath);

		int width, height, nrComponents;
		unsigned char *pixelData;

	};
}