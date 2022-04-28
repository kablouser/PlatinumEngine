#pragma once

// OpenGL types
#include <GL/glew.h>
// For supporting ImGui types
#include <imgui.h>
#include <string>
#include <vector>

namespace PlatinumEngine
{
	// forward declare for Create function
	struct PixelData;

	class Texture
	{
	public:

		Texture();

		~Texture();

		void Create(const PixelData& pixelData);

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
		bool Create(const std::string& filePath);

		int width, height;
//     N=#comp     components
//       1           grey
//       2           grey, alpha
//       3           red, green, blue
//       4           red, green, blue, alpha
		int numberOfComponents;
		unsigned char *pixelData;
	};
}