#include <OpenGL/Texture.h>
// for checking errors
#include <OpenGL/GLCheck.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace PlatinumEngine
{
	Texture::Texture() :
			_textureHandle(0)
	{}

	Texture::~Texture()
	{
		GL_CHECK(glDeleteTextures(1,&_textureHandle));
	}

	void Texture::Create(GLsizei width, GLsizei height, const void* pixelData)
	{
		if(_textureHandle == 0)
			GL_CHECK(glGenTextures(1, &_textureHandle));

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));
		GL_CHECK(glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, pixelData));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}

	GLuint Texture::GetOpenGLHandle() const
	{
		return _textureHandle;
	}

	ImTextureID Texture::GetImGuiHandle() const
	{
		return (void*)(intptr_t)(_textureHandle);
	}

	void Texture::Bind() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));
	}

	void Texture::Unbind() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}

	///-------------------------------------------------------------
	/// class PixelData
	///-------------------------------------------------------------

	void PixelData::Create(const std::string& filePath)
	{
		/*
		if(_textureHandle == 0)
			GL_CHECK(glGenTextures(1, &_textureHandle));
*/
		pixelData = stbi_load(filePath.c_str(),
									&width, &height,
								 &nrComponents, 0);

		if(pixelData)
		{/*
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			GL_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));
			GL_CHECK(glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
					format, GL_UNSIGNED_BYTE, pixelData));
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
*/
			//stbi_image_free(pixelData);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << filePath << std::endl;
			//stbi_image_free(pixelData);
		}
	}

	PixelData::PixelData():pixelData(0), height(0), width(0)
	{

	}

	PixelData::~PixelData()
	{
		stbi_image_free(pixelData);
	}



}