//
// Created by Matt on 03/03/2022.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

// Post processing options: http://assimp.sourceforge.net/lib_html/postprocess_8h.html

// For now to avoid anything weird explicitly only allow these files to be loaded
const std::vector<std::string> ALLOWED_EXTENSIONS{"obj"};

namespace PlatinumEngine
{
	namespace Common
	{
		/**
		 * Loads a mesh file into positions, normals
		 * @param filePath : location of file
		 * @param positions : where positions are stored
		 * @param normals  : where normals are stored
		 */
		void LoadMesh(const std::string &filePath, std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &textureCoords);

		/**
		 * Returns the extensions of the given filepath
		 * @param filePath
		 * @return : Extension as string
		 */
		std::string GetExtension(const std::string &filePath);

		/**
		 * Checks is extension exists in list of allowed extensions
		 * @param extension
		 * @return : True if extensions ok, false is bad
		 */
		bool ExtensionAllowed(const std::string &extension);

		/**
		 * Converts a mesh into positions, normals
		 * @param mesh
		 */
		void ConvertMesh(aiMesh *mesh, std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<glm::vec2> &textureCoords);
	}
}
