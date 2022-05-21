#include <OpenGL/Framebuffer.h>
// for checking errors
#include <OpenGL/GLCheck.h>
// for printing errors
#include <Logger/Logger.h>

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
			PLATINUM_WARNING("Framebuffer not complete!");

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

namespace PlatinumEngine
{
	DepthMapFrameBuffer::DepthMapFrameBuffer() : _framebufferHandle(0) {}

	DepthMapFrameBuffer::~DepthMapFrameBuffer()
	{
		GL_CHECK(glDeleteFramebuffers(1, &_framebufferHandle));
	}

	bool DepthMapFrameBuffer::Create(GLsizei width, GLsizei height)
	{
		if (_framebufferHandle == 0)
			GL_CHECK(glGenFramebuffers(1, &_framebufferHandle));
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle));

		// There is no colour attachment in a depth map frame buffer
		depthMap.CreateDepthMap(width, height);
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.GetOpenGLHandle(), 0));

		// Don't actually need a colour buffer for rendering so tell OpenGL no colour data will be rendered
		GL_CHECK(glDrawBuffer(GL_NONE));
		GL_CHECK(glReadBuffer(GL_NONE));

		// Job done, reset global state
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true; // Idk what else to return here?
	}

	void DepthMapFrameBuffer::Bind()
	{
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle));
	}

	void DepthMapFrameBuffer::Unbind()
	{
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	GLuint DepthMapFrameBuffer::GetFrameBufferHandle() const
	{
		return _framebufferHandle;
	}
}