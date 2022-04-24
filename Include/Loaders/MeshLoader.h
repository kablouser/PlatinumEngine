//
// Created by Matt on 03/03/2022.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Loaders/LoaderCommon.h>
#include <Logger/Logger.h>
#include <OpenGL/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

// Post processing options: http://assimp.sourceforge.net/lib_html/postprocess_8h.html

namespace PlatinumEngine
{
	namespace Loaders
	{
		/**
		 * Loads a mesh file into positions, normals
		 * @param filePath : location of file
		 * @param outPositions : where positions are stored
		 * @param outNormals  : where normals are stored
		 * @param outTextureCoords : where texture coords are stored
		 */
		void LoadMesh(const std::string &filePath, std::vector<glm::vec3> &outPositions, std::vector<glm::vec3> &outNormals, std::vector<glm::vec2> &outTextureCoords);

		/**
		 * Convert an assimp mesh to individual positon, normal, texture coord vectors
		 * @param mesh : assimp mesh of data
		 * @param outPositions : positions of vertices from mesh
		 * @param outNormals : normals of vertices from mesh
		 * @param outTextureCoords : texture coords of vertices from mesh
		 */
		void ConvertMesh(aiMesh *mesh, std::vector<glm::vec3> &outPositions, std::vector<glm::vec3> &outNormals, std::vector<glm::vec2> &outTextureCoords);

		/**
		 * Loads a file into a mesh
		 * @param filePath : Location of file
		 * @param JoinVertices : Will join vertices so the mesh contains unique vertices only, default true
		 * @return : Mesh data structure
		 */
		Mesh LoadMesh(const std::string &filePath, bool JoinVertices=true);

		/**
		 * AddInternal new mesh data to an existing mesh
		 * @param outMesh : The mesh structure used for rendering with new vertices and indices added
		 * @param mesh : The assimp mesh data
		 * @param offset : This is the offset of indices for the current mesh, should start at 0
		 */
		void AddMeshData(Mesh &outMesh, aiMesh *mesh, unsigned int &offset);
	}
}

