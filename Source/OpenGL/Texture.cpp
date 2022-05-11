#include <OpenGL/Texture.h>
#include <Logger/Logger.h>
// for checking errors
#include <OpenGL/GLCheck.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace PlatinumEngine
{
	Texture::Texture() :
			_textureHandle(0),
			width(0), height(0)
	{}

	Texture::~Texture()
	{
		GL_CHECK(glDeleteTextures(1,&_textureHandle));
	}

	Texture::Texture(Texture&& moveFrom) noexcept :
		fileName(std::move(moveFrom.fileName)), _textureHandle(moveFrom._textureHandle),
		width(moveFrom.width), height(moveFrom.height)
	{
		moveFrom._textureHandle = 0;
		moveFrom.width = moveFrom.height = 0;
	}

	Texture& Texture::operator=(Texture&& moveFrom) noexcept
	{
		fileName = std::move(moveFrom.fileName);
		_textureHandle = moveFrom._textureHandle;
		width = moveFrom.width;
		height = moveFrom.height;
		moveFrom._textureHandle = 0;
		moveFrom.width = moveFrom.height = 0;
		return *this;
	}

	void Texture::Create(GLsizei inWidth, GLsizei inHeight)
	{
		width = inWidth;
		height = inHeight;
		if(_textureHandle == 0)
			GL_CHECK(glGenTextures(1, &_textureHandle));

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));
		GL_CHECK(glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, nullptr));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::Create(const PixelData& pixelData)
	{
		width = pixelData.width;
		height = pixelData.height;
		if(_textureHandle == 0)
			GL_CHECK(glGenTextures(1, &_textureHandle));

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));

		switch(pixelData.numberOfComponents)
		{
		case 4:
			GL_CHECK(glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, pixelData.pixelData));
			break;
		case 3:
			GL_CHECK(glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
					GL_RGB, GL_UNSIGNED_BYTE, pixelData.pixelData));
			break;
		default:
			PLATINUM_WARNING_STREAM << "Number of components not supported: " << pixelData.numberOfComponents;
			break;
		}

		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		this->width = width;
		this->height = height;
	}

	void Texture::CreateCubeMap(std::vector<std::string> faces)
	{

		// generate new texture ID for the cube map texture
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		// fetch the images for all the faces of the cube

		for(int i = 0; i<faces.size();i++)
		{

			PixelData pixelData;

			pixelData.Create(faces[i]);

			if(pixelData.pixelData)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, GL_RGB, pixelData.width, pixelData.height, 0,
						GL_RGB, GL_UNSIGNED_BYTE, pixelData.pixelData
						);

			}
			else
			{
				PLATINUM_WARNING(std::string("Cannot fetch the texture image: ").append(faces[i]));
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		_textureHandle = textureID;

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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

	void Texture::BindCubeMap() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, _textureHandle));
	}

	void Texture::UnbindCubeMap() const
	{
		GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	///-------------------------------------------------------------
	/// class PixelData
	///-------------------------------------------------------------

	PixelData::PixelData(): pixelData(nullptr), height(0), width(0), numberOfComponents(0)
	{
	}

	PixelData::~PixelData()
	{
		stbi_image_free(pixelData);
	}

	bool PixelData::Create(const std::string& filePath)
	{
		pixelData = stbi_load(filePath.c_str(), &width, &height, &numberOfComponents, 0);

		if (pixelData == nullptr)
		{
			height = width = numberOfComponents = 0; // reset other fields
			PLATINUM_ERROR_STREAM << "Failed to load image at path: " << filePath;
			return false;
		}

		return true;
	}
}