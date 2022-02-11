#include <catch2/catch.hpp>
#include <RasterRenderer/RasterRenderer.h>
#include <ShaderInput/ShaderInput.h>

// Read the docs
// https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md

TEST_CASE("ShaderInput", "[OpenGL Abstraction]")
{
	// curly brackets control when objects are destroyed
	{
		PlatinumEngine::RasterRenderer renderer;
		{
			PlatinumEngine::ShaderInput shaderInput;

			/*
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
			 */

			auto state = shaderInput.ReadState();

			REQUIRE(state._vertexBufferObject == 0);
			REQUIRE(state._vertexArrayObject == 0);
			REQUIRE(state._elementBufferObject == 0);

			REQUIRE(state._verticesSize == 0);
			REQUIRE(state._indicesSize == 0);
			REQUIRE(state._vertexAttributesSize == 0);

			// TODO more test cases
		}
	}
}