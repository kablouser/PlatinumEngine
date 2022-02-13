#include <catch2/catch.hpp>
#include <RasterRenderer/RasterRenderer.h>
#include <OpenGL/ShaderInput.h>
#include <iostream>

#include <GL/glew.h>

#include <OpenGL/GLCheck.h>

// Read the docs
// https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md

using namespace PlatinumEngine;

struct MyVertex
{
	sf::Glsl::Vec3 position;
	sf::Glsl::Vec3 normal;
	sf::Glsl::Vec2 textureCoordinate;
};

struct OtherVertex
{
	sf::Glsl::Vec3 position;
};

TEST_CASE("ShaderInput", "[OpenGL Abstraction]")
{
	RasterRenderer renderer;

	{
		ShaderInput shaderInput;

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

		REQUIRE(state._vertexType != std::type_index(typeid(MyVertex)));
		REQUIRE(state._indexType != GL_UNSIGNED_INT);

		REQUIRE(state._verticesSize == 0);
		REQUIRE(state._indicesSize == 0);
		REQUIRE(state._vertexAttributesSize == 0);

		/*
		sf::Glsl::Vec3 position;
		sf::Glsl::Vec3 normal;
		sf::Glsl::Vec2 textureCoordinate;
		 */
		std::vector<MyVertex> vertices = {
				{{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }},
				{{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }},
				{{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }}};
		shaderInput.SetVertices(vertices);
		state = shaderInput.ReadState();

		REQUIRE(state._vertexBufferObject != 0);
		REQUIRE(state._vertexArrayObject == 0);
		REQUIRE(state._elementBufferObject == 0);

		REQUIRE(state._vertexType == std::type_index(typeid(MyVertex)));
		REQUIRE(state._indexType != GL_FLOAT);

		REQUIRE(state._verticesSize == 3);
		REQUIRE(state._indicesSize == 0);
		REQUIRE(state._vertexAttributesSize == 0);

		shaderInput.SetVertexAttributes<MyVertex>({
				{ GL_FLOAT, 3, offsetof(MyVertex, position) },
				{ GL_FLOAT, 3, offsetof(MyVertex, normal) },
				{ GL_FLOAT, 2, offsetof(MyVertex, textureCoordinate) }
		});
		state = shaderInput.ReadState();

		REQUIRE(state._vertexBufferObject != 0);
		REQUIRE(state._vertexArrayObject != 0);
		REQUIRE(state._elementBufferObject == 0);

		REQUIRE(state._vertexType == std::type_index(typeid(MyVertex)));
		REQUIRE(state._indexType != GL_UNSIGNED_INT);

		REQUIRE(state._verticesSize == 3);
		REQUIRE(state._indicesSize == 0);
		REQUIRE(state._vertexAttributesSize == 3);

		std::vector<GLuint> indices = { 0, 1, 2 };
		shaderInput.SetIndices(indices);
		state = shaderInput.ReadState();

		REQUIRE(state._vertexBufferObject != 0);
		REQUIRE(state._vertexArrayObject != 0);
		REQUIRE(state._elementBufferObject != 0);

		REQUIRE(state._vertexType == std::type_index(typeid(MyVertex)));
		REQUIRE(state._indexType == GL_UNSIGNED_INT);

		REQUIRE(state._verticesSize == 3);
		REQUIRE(state._indicesSize == 3);
		REQUIRE(state._vertexAttributesSize == 3);

		shaderInput.SetVertexAttributes<MyVertex>({
				{ GL_FLOAT, 3, offsetof(MyVertex, position) },
				{ GL_FLOAT, 3, offsetof(MyVertex, normal) }
		});
		state = shaderInput.ReadState();

		REQUIRE(state._vertexBufferObject != 0);
		REQUIRE(state._vertexArrayObject != 0);
		REQUIRE(state._elementBufferObject != 0);

		REQUIRE(state._vertexType == std::type_index(typeid(MyVertex)));
		REQUIRE(state._indexType == GL_UNSIGNED_INT);

		REQUIRE(state._verticesSize == 3);
		REQUIRE(state._indicesSize == 3);
		REQUIRE(state._vertexAttributesSize == 2);

		shaderInput.SetVertices<OtherVertex>({});
		shaderInput.SetVertexAttributes<OtherVertex>({});
		shaderInput.SetIndices<GLubyte>({});
		state = shaderInput.ReadState();

		REQUIRE(state._vertexBufferObject != 0);
		REQUIRE(state._vertexArrayObject != 0);
		REQUIRE(state._elementBufferObject != 0);

		REQUIRE(state._vertexType == std::type_index(typeid(OtherVertex)));
		REQUIRE(state._indexType == GL_UNSIGNED_BYTE);

		REQUIRE(state._verticesSize == 0);
		REQUIRE(state._indicesSize == 0);
		REQUIRE(state._vertexAttributesSize == 0);

		shaderInput.DeleteVertexAttributes();
		shaderInput.DeleteVertices();
		shaderInput.DeleteIndices();
		state = shaderInput.ReadState();

		REQUIRE(state._vertexBufferObject == 0);
		REQUIRE(state._vertexArrayObject == 0);
		REQUIRE(state._elementBufferObject == 0);

		REQUIRE(state._vertexType != std::type_index(typeid(MyVertex)));
		REQUIRE(state._indexType != GL_UNSIGNED_INT);

		REQUIRE(state._verticesSize == 0);
		REQUIRE(state._indicesSize == 0);
		REQUIRE(state._vertexAttributesSize == 0);
	}


	{
		ShaderInput shaderInput2({
						{{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }},
						{{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }},
						{{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }}},
				{ 0, 1, 2 });
		auto state = shaderInput2.ReadState();

		REQUIRE(state._vertexBufferObject != 0);
		REQUIRE(state._vertexArrayObject != 0);
		REQUIRE(state._elementBufferObject != 0);

		REQUIRE(state._vertexType == std::type_index(typeid(ShaderInput::Vertex)));
		REQUIRE(state._indexType == GL_UNSIGNED_INT);

		REQUIRE(state._verticesSize == 3);
		REQUIRE(state._indicesSize == 3);
		REQUIRE(state._vertexAttributesSize == 3);
	}
}