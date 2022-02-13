#pragma once

#include <vector>
// SFML uses some global state to track its GlResources
#include <SFML/Window/GlResource.hpp>
// OpenGL compatible structs
#include <SFML/Graphics/Glsl.hpp>
// OpenGL types
#include <GL/glew.h>
#include <OpenGL/GLEnumInfo.h>
// Catching errors, not strictly necessary
#include <type_traits>
#include <typeindex>
#include <cassert>

namespace PlatinumEngine
{
	/**
	 * Represents data that can be used in shader's input (*.glsl in keyword).
	 * Abstraction for OpenGL's VBO, VAO, EBO.
	 *
	 * Inherits from sf::GlResource to help SFML with OpenGL context cleanup.
	 */
	class ShaderInput : sf::GlResource
	{
	public:

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

		/**
		 * A common vertex structure
		 */
		struct Vertex
		{
			sf::Glsl::Vec3 position;
			sf::Glsl::Vec3 normal;
			sf::Glsl::Vec2 textureCoordinate;
		};

		/**
		 * Complete copy of ShaderInput's state variables. For testing purposes.
		 */
		struct ShaderInputState
		{
			GLuint _vertexBufferObject;
			GLuint _vertexArrayObject;
			GLuint _elementBufferObject;
			std::type_index _vertexType;
			GLenum _indexType;
			GLsizeiptr _verticesSize, _indicesSize;
			GLuint _vertexAttributesSize;
		};

		/**
		 * Default constructor. No data will be allocated/transferred to GPU.
		 */
		ShaderInput();

		/**
		 * Primary constructor. Performs the setup for the most common type of input.
		 */
		ShaderInput(
				const std::vector<Vertex>& vertices,
				const std::vector<GLuint>& indices,
				GLenum verticesUsage = GL_STATIC_DRAW,
				GLenum indicesUsage = GL_STATIC_DRAW);

		/*
		 * Can't do deep copy of all contents.
		 */
		ShaderInput(ShaderInput&) = delete;

		/*
		 * Default move constructor should be good enough. Transfers ownership of data.
		 */
		ShaderInput(ShaderInput&&) = default;

		/**
		 * Destructor
		 * Deletes the OpenGL resources in-use by this object.
		 */
		~ShaderInput();

		//--------------------------------------------------------------------------------------------------------------
		// Templates. Main purpose is convenience and safe guard against typos.
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Send new data to the GPU. Usually vertex positions, normals, uvs.
		 * If SetIndices is never used, then data is iterated in order of input vertices.
		 * Must call this before calling SetVertexAttributes for the first time.
		 * @tparam T Must be plain old data (POD) struct/class
		 * @param vertices Generic data to send
		 * @param usage How this data is being used, must be one of {GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
		 * GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY}
		 */
		template<typename T>
		void SetVertices(const std::vector<T>& vertices, GLenum usage = GL_STATIC_DRAW)
		{
			SetVertices(vertices.size(), vertices.data(), std::type_index(typeid(T)), sizeof(T), usage);
		}

		/**
		 * You must call SetVertices at least once before this function.\n
		 *
		 * Configures the vertex structure layout to expect. Example usage:
		 * @code
		 * struct Vertex { sf::Glsl::Vec3 position; sf::Glsl::Vec3 normal; sf::Glsl::Vec3 uv; };
		 *
		 * SetVertexAttributes<Vertex>({
		 * 		{GL_FLOAT, 3, offsetof(Vertex,position)},
		 * 		{GL_FLOAT, 3, offsetof(Vertex,normal)},
		 * 		{GL_FLOAT, 2, offsetof(Vertex,uv)}
		 * });
		 * @endcode
		 * Pay very close attention to the field types you're using. Make sure it's the same size as OpenGL types.
		 * @tparam T Must be the same type used in call to SetVertices
		 * @param vertexAttributes List of attributes in vertexType
		 */
		template<typename T>
		void SetVertexAttributes(const std::vector<VertexAttribute>& vertexAttributes)
		{
			// You shouldn't change vertex type while there are vertices on the GPU
			// First clear/delete them by either: SetVertices(std::vector<T>()); or DeleteVertices();
			assert(_vertexType == std::type_index(typeid(T)));
			SetVertexAttributes(sizeof(T), vertexAttributes);
		}

		/**
		 * Send new indexing data to the GPU.
		 * Indexing represents how GPU will iterate through the vertices.
		 * Also enables indexed drawing.
		 * Use this function to automatically find size and type GLenum.
		 * @tparam T Must be one of {GLubyte, GLushort, GLuint}
		 * @param indices Indexing data to send
		 * @param usage How this data is being used, must be one of {GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
		 * GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY}
		 */
		template<typename T>
		void SetIndices(const std::vector<T>& indices, GLenum usage = GL_STATIC_DRAW)
		{
			// Compile-time check if T is one of the allowed types
			static_assert(
					std::is_same<T, GLubyte>::value ||
					std::is_same<T, GLushort>::value ||
					std::is_same<T, GLuint>::value);
			SetIndices(indices.size(), indices.data(), GetGLenum<T>(), usage);
		}

		//--------------------------------------------------------------------------------------------------------------
		// Draw calls.
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Calls the appropriate drawing function,
		 * depending on if indexing data is used or not.
		 * @param drawingMode Changes the shape assembly stage of OpenGL's pipeline for drawing this input.
		 * Must be one of {GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY,
		 * GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY,
		 * GL_TRIANGLES_ADJACENCY, GL_PATCHES}.
		 */
		void Draw(GLenum drawingMode = GL_TRIANGLES) const;

		//--------------------------------------------------------------------------------------------------------------
		// Get sizes.
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * @return number of vertices stored in GPU by this object
		 */
		GLsizeiptr GetVerticesSize() const;

		/**
		 * @return number of attributes enabled by this object
		 */
		GLuint GetVertexAttributesSize() const;

		/**
		 * @return number of indexes stored in GPU by this object
		 */
		GLsizeiptr GetIndicesSize() const;

		//--------------------------------------------------------------------------------------------------------------
		// Has checks
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * @return true iff vertices data is stored on GPU, false otherwise
		 */
		bool HasVertices() const;

		/**
		 * @return true iff vertex attributes are created, false otherwise
		 */
		bool HasVertexAttributes() const;

		/**
		 * @return true iff indexing data is stored on GPU, false otherwise
		 */
		bool HasIndices() const;

		//--------------------------------------------------------------------------------------------------------------
		// Deletes
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Delete the vertices sent to the GPU
		 */
		void DeleteVertices();

		void DeleteVertexAttributes();

		/**
		 * Delete the indices sent to the GPU. Also disables indexed drawing.
		 */
		void DeleteIndices();

		//--------------------------------------------------------------------------------------------------------------
		// Reading operations. For testing purposes.
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * @return copy of all internal state variables
		 */
		ShaderInputState ReadState() const;

	private:

		/**
		 * OpenGL object ID representing the vertices stored on the GPU
		 */
		GLuint _vertexBufferObject;
		/**
		 * OpenGL object ID representing the vertex attributes
		 */
		GLuint _vertexArrayObject;
		/**
		 * OpenGL object ID representing the drawing indices
		 */
		GLuint _elementBufferObject;

		/**
		 * CPU type of the vertices stored in CPU
		 */
		std::type_index _vertexType;

		/**
		 * OpenGL data type of the indices stored in GPU
		 */
		GLenum _indexType;

		/**
		 * size of data stored in GPU
		 */
		GLsizeiptr _verticesSize, _indicesSize;

		/**
		 * number of vertex attributes enabled
		 */
		GLuint _vertexAttributesSize;

		//--------------------------------------------------------------------------------------------------------------
		// Primary important functions. Unlikely you'd need to call these functions directly.
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * Send new data to the GPU. Usually vertex positions, normals, uvs.
		 * If SetIndices is never used, then data is iterated in order of input vertices.
		 * @param verticesSize Number of elements in the vertices array
		 * @param vertices Start of the vertices array
		 * @param typeOfVertex type index of Vertex, obtained through std::type_index(typeinfo(T))
		 * @param sizeOfVertex Size of vertex data structure
		 * @param usage How this data is being used, must be one of {GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
		 * GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY}
		 */
		void SetVertices(
				GLsizeiptr verticesSize,
				const void* vertices,
				std::type_index typeOfVertex,
				size_t sizeOfVertex,
				GLenum usage = GL_STATIC_DRAW);

		/**
		 * Configures the vertex structure layout on GPU.
		 * @param sizeOfVertex number of bytes of Vertex, obtained through sizeof(T)
		 * @param vertexAttributes List of attributes in vertexType
		 */
		void SetVertexAttributes(
				size_t sizeOfVertex,
				const std::vector<VertexAttribute>& vertexAttributes);

		/**
		 * Send new indexing data to the GPU.
		 * Indexing represents how GPU will iterate through the vertices.
		 * Also enables indexed drawing.
		 * @param indicesSize Number of elements in the indices array
		 * @param indices Start of the indices array
		 * @param indexType Must be one of {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT}
		 * @param usage How this data is being used, must be one of {GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
		 * GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY}
		 */
		void SetIndices(
				GLsizeiptr indicesSize,
				const void* indices,
				GLenum indexType = GL_UNSIGNED_INT,
				GLenum usage = GL_STATIC_DRAW);
	};
}
