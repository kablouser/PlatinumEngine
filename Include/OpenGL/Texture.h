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

		void Create(GLsizei _width, GLsizei _height, const void* pixelData = nullptr, int nrComponents = 3);

		void CreateCubeMap(std::vector<std::string> faces);

		GLuint GetOpenGLHandle() const;
		ImTextureID GetImGuiHandle() const;
		void Bind() const;
		void Unbind() const;

		void BindCubeMap() const;
		void UnbindCubeMap() const;

		std::string fileName;
		float width, height;
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