#pragma once

// SFML uses some global state to track its GlResources
#include <SFML/Window/GlResource.hpp>
#include <vector>

// glew.h replaces gl.h. So don't #include <SFML/OpenGL.hpp>
#include <GL/glew.h>

namespace PlatinumEngine
{
	/**
	 * Represents data that can be used in shader's input (*.glsl in keyword).
	 * Abstraction for OpenGL's VBO, VAO, EBO.
	 *
	 * Inherits from sf::GlResource to help SFML with OpenGL context cleanup
	 */
	class ShaderInput : sf::GlResource
	{
	public:
		/**
		 * Hint to graphics driver.
		 * How frequently data is expected to change.
		 */
		enum Usage
		{
			/**
			 * Use the same usage as before
			 */
			PreviousUsage,
			/**
			 * Constantly changing data
			 */
			Stream,
			/**
			 * Occasionally changing data
			 */
			Dynamic,
			/**
			 * Rarely changing data
			 */
			Static
		};

		/**
		 * Construct with empty data
		 */
		ShaderInput();

		/**
		 * Copy constructor
		 * Copy others object's data
		 * @param copy target to copy from
		 */
		ShaderInput(const ShaderInput& copy);

		/**
		 * Move constructor
		 * Move data from one class to another to transfer ownership
		 * @param moveFrom target to move data out of
		 */
		ShaderInput(ShaderInput&& moveFrom) noexcept;

		/**
		 * Destructor
		 * Deletes the OpenGL resources in-use by this object.
		 */
		~ShaderInput();

		/**
		 * Send new data to the GPU. Usually vertex positions, normals, uvs.
		 * If SetIndices is never used, then data is iterated in order of input vertices.
		 * @tparam T \n
		 * Must be a OpenGL data type, see https://www.khronos.org/opengl/wiki/OpenGL_Type.\n
		 * And must be a type allowed in glVertexAttribPointer, see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
		 * @param vertices generic data to send
		 * @param usage how this data is used
		 */
		template<typename T>
		void SetVertices(const std::vector<T>& vertices, Usage usage = Usage::PreviousUsage);

		/**
		 * Send new indexing data to the GPU.
		 * Indexing represents how GPU will iterate through the vertices.
		 * Also enables indexed drawing.
		 * @tparam T Must be one of {GLubyte, GLushort, GLuint}
		 * @param indices indexing data to send
		 * @param usage how this data is used
		 */
		template<typename T>
		void SetIndices(const std::vector<T>& indices, Usage usage = Usage::PreviousUsage);

		/**
		 * Delete the vertices sent to the GPU
		 */
		void DeleteVertices();

		/**
		 * Delete the indices sent to the GPU
		 */
		void DeleteIndices();

		/**
		 * @return number of vertices stored in GPU by this object
		 */
		std::size_t GetVerticesSize() const;

		/**
		 * @return number of indexes stored in GPU by this object
		 */
		std::size_t GetVIndicesSize() const;

		/**
		 * binds the vertex array object, which in turn
		 * binds the vertex buffer object and element buffer object
		 * (if they exist)
		 */
		void Bind() const;

		/**
		 * calls the appropriate drawing function
		 * depending on if indexing data is used or not
		 */
		void Draw() const;

		/**
		 * Converts a OpenGL type into it's respective GLenum if possible.
		 * Except {GLfixed, GLhalf}, because it's the same as {GLint, GLushort } respectively as far as C++ templates are concerned.
		 * @tparam T Must be a OpenGL data type, see https://www.khronos.org/opengl/wiki/OpenGL_Type.
		 * @return GLenum representing the same type iff type has a GLenum, otherwise 0
		 */
		template<typename T>
		static GLenum GetGLenum();

	private:

		GLuint
				_vertexBufferObject,
				_vertexArrayObject,
				_elementBufferObject;

		std::size_t
				_verticesSize,
				_indicesSize;

		Usage
			_verticesUsage,
			_indicesUsage;
	};
}
