//
// Created by Shawn on 2022/4/5.
//
#pragma once

#include <OpenGL/Mesh.h>
#include <AssetDatabase/AssetDatabase.h>
#include <imgui.h>

namespace PlatinumEngine
{
	class AssetHelper
	{
	public:
		AssetHelper();
		~AssetHelper();
		/**
		 * Responsible for the pop up window opened by
		 * the render component in Inspector Window
		 */
		 bool ShowGuiWindow();

		 Mesh* GetMesh();
		/**
		 * Loop through the database and
		 * save the filepath of the _asset to _assetFilePath
		 * save the mesh
		 */
		void LoopDatabase();



	private:
		Mesh* _mesh = nullptr;
		AssetDatabase _assetDatabase;
		Asset _asset;
		char _meshBuffer[128];

	};
}