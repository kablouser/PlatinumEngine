#include <OpenGL/Texture.h>
// for checking errors
#include <OpenGL/GLCheck.h>

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
}