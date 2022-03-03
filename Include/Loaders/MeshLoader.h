//
// Created by Matt on 03/03/2022.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Loaders/LoaderCommon.h>

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
		 * @param positions : where positions are stored
		 * @param normals  : where normals are stored
		 */
		void LoadMesh(const std::string &filePath, std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &textureCoords);

		/**
		 * Converts a mesh into positions, normals
		 * @param mesh
		 */
		void ConvertMesh(aiMesh *mesh, std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &textureCoords);
	}
}

