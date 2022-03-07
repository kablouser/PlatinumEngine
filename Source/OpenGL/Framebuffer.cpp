#include <OpenGL/Framebuffer.h>
// for checking errors
#include <OpenGL/GLCheck.h>
// for printing errors
#include <iostream>

namespace PlatinumEngine
{
	Framebuffer::Framebuffer() :
			_framebufferHandle(0),
			_depthAndStencilHandle(0)
	{
	}

	Framebuffer::~Framebuffer()
	{
		GL_CHECK(glDeleteFramebuffers(1, &_framebufferHandle));
		GL_CHECK(glDeleteRenderbuffers(1, &_depthAndStencilHandle));
	}

	bool Framebuffer::Create(GLsizei width, GLsizei height, bool attachDepthAndStencil)
	{
		if (_framebufferHandle == 0)
			GL_CHECK(glGenFramebuffers(1, &_framebufferHandle));
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle));

		// color attachment
		_colorTexture.Create(width, height);
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTexture.GetOpenGLHandle(), 0));

		if (attachDepthAndStencil)
		{
			if (_depthAndStencilHandle == 0)
				GL_CHECK(glGenRenderbuffers(1, &_depthAndStencilHandle));

			GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, _depthAndStencilHandle));
			GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
			GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));

			// depth and stencil attachment
			GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthAndStencilHandle));
		}

		GLenum status;
		GL_CHECK(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cerr << "Framebuffer not complete!" << std::endl;

		// reset global state
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return status == GL_FRAMEBUFFER_COMPLETE;
	}

	void Framebuffer::Bind() const
	{
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle));
	}

	void Framebuffer::Unbind() const
	{
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	GLuint Framebuffer::GetFramebufferHandle() const
	{
		return _framebufferHandle;
	}

	const Texture& Framebuffer::GetColorTexture() const
	{
		return _colorTexture;
	}

	GLuint Framebuffer::GetDepthAndStencilHandle() const
	{
		return _depthAndStencilHandle;
	}
}