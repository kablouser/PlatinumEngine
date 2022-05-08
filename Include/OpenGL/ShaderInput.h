#pragma once

#include <vector>
#include <glm/glm.hpp>
// OpenGL types
#include <GL/glew.h>
#include <OpenGL/GLEnumInfo.h>
#include <OpenGL/Mesh.h>
// Catching errors, not strictly necessary
#include <type_traits>
#include <cassert>

namespace PlatinumEngine
{
	/**
	 * Not for uniform variables.
	 * Represents data that can be used in shader's input (*.glsl in keyword).
	 * Abstraction for OpenGL's VBO, VAO, EBO.
	 *
	 * Inherits from sf::GlResource to help SFML with OpenGL context cleanup.
	 */
	class ShaderInput
	{
	public:

		//--------------------------------------------------------------------------------------------------------------
		// Data structures
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * A vertex is given to every "thread" in GPU.
		 * A vertex attribute is one of these in *.glsl: (location = 0) in vec3 position;
		 * But in CPU, the structure will look different.
		 * This vertex attribute struct describes the CPU side.
		 */
		struct VertexAttribute
		{
			/**
			 * Must be one of {GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT,
			 * GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED,
			 * GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV}
			 */
			GLenum elementType;
			/**
			 * Must be in range [1,4].
			 * For example, sf::Glsl::Vec3 has 3 components.
			 */
			GLint numberOfComponents;
			/**
			 * Calculated using: offsetof(VertexType, thisAttribute);
			 * https://en.cppreference.com/w/cpp/types/offsetof
			 */
			unsigned long long int offset;
		};

		//--------------------------------------------------------------------------------------------------------------
		// Constructors/Destructors
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Default constructor. No data will be allocated/transferred to GPU.
		 */
		ShaderInput();

		/**
		 * Primary constructor. Performs the setup for the most common type of input.
		 * @param vertices
		 * @param indices
		 */
		ShaderInput(
				const std::vector<Vertex>& vertices,
				const std::vector<GLuint>& indices);

		// Move-only object, no copying

		// Delete copy constructor
		ShaderInput(ShaderInput&) = delete;

		// Delete copy assignment
		ShaderInput& operator=(const ShaderInput&) = delete;

		// Move constructor
		ShaderInput(ShaderInput&& other) noexcept;

		// Move assignment
		ShaderInput& operator=(ShaderInput&& other) noexcept;

		/**
		 * Destructor
		 * Deletes the OpenGL resources in-use by this object.
		 */
		~ShaderInput();

		//--------------------------------------------------------------------------------------------------------------
		// Public functions.
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * A more generic way of setting data
		 * @tparam VertexType vertex data structure, must be plain old data (POD)
		 * @tparam IndexType must be one of {GLubyte, GLushort, GLuint}
		 * @param vertexAttributes see ShaderInput::VertexAttribute
		 * @param vertices
		 * @param indices
		 * @param usage must be one of \n
		 * {GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,\n
		 * GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,\n
		 * GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY}
		 */
		template<typename VertexType, typename IndexType>
		void SetGeneric(
				const std::vector<VertexAttribute>& vertexAttributes,
				const std::vector<VertexType>& vertices,
				const std::vector<IndexType>& indices,
				GLenum usage = GL_STATIC_DRAW)
		{
			// IndexType must be one of {GLubyte, GLushort, GLuint}
			static_assert(
					std::is_same<IndexType, GLubyte>::value ||
					std::is_same<IndexType, GLushort>::value ||
					std::is_same<IndexType, GLuint>::value);
			Set(
					vertexAttributes,
					sizeof(VertexType), vertices.size(), vertices.data(),
					GetGLenum<IndexType>(), sizeof(IndexType), indices.size(), indices.data(),
					usage);
		}

		/**
		 * USE THIS!
		 * Set data using the common vertex structure.
		 * @param vertices
		 * @param indices
		 */
		void Set(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

		/**
		 * DON't USE THIS!
		 * For setting buffer of animation shader only.
		 * @param vertices
		 * @param indices
		 */
		void Set(const std::vector<AnimationVertex>& vertices, const std::vector<GLuint>& indices);

		/**
		 * Delete data sent to the GPU
		 */
		void Clear();

		/**
		 * Inputs the data held by this object in the current pipeline.
		 * @param drawingMode Changes the shape assembly stage of OpenGL's pipeline for drawing this input.
		 * Must be one of {GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY,
		 * GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY,
		 * GL_TRIANGLES_ADJACENCY, GL_PATCHES}.
		 */
		void Draw(GLenum drawingMode = GL_TRIANGLES) const;

		/**
		 * @return true if data is allocated on GPU, false otherwise
		 */
		bool IsCreated() const;

	private:

		// OpenGL object ID representing the vertex attributes
		GLuint _vertexArrayObject;
		// OpenGL object ID representing the vertices stored on the GPU
		GLuint _vertexBufferObject;
		// OpenGL object ID representing the drawing indices
		GLuint _elementBufferObject;

		// type of index in GLenum form, must be one of {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT}
		GLenum _typeOfIndex;
		// number of vertex attributes enabled
		GLuint _vertexAttributesLength;
		// number of elements to use in draw call
		GLsizeiptr _drawLength;

		//--------------------------------------------------------------------------------------------------------------
		// Private functions.
		//--------------------------------------------------------------------------------------------------------------

		void Set(
				const std::vector<VertexAttribute>& vertexAttributes,

				size_t sizeOfVertex,
				size_t verticesLength,
				const void* vertices,

				GLenum typeOfIndex,
				size_t sizeOfIndex,
				size_t indicesLength,
				const void* indices,

				GLenum usage = GL_STATIC_DRAW);
	};
}
