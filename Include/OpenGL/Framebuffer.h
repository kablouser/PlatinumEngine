#pragma once

// framebuffer needs color-attachment, which is a texture
#include <OpenGL/Texture.h>
// OpenGL types
#include <GL/glew.h>

namespace PlatinumEngine
{
	class Framebuffer
	{
	public:

		Framebuffer();
		~Framebuffer();

		bool Create(GLsizei width, GLsizei height, bool attachDepthAndStencil = true);

		void Bind() const;
		void Unbind() const;

		GLuint GetFramebufferHandle() const;
		const Texture& GetColorTexture() const;
		GLuint GetDepthAndStencilHandle() const;

	private:

		GLuint _framebufferHandle;
		GLuint _depthAndStencilHandle;
		Texture _colorTexture;

	};
}