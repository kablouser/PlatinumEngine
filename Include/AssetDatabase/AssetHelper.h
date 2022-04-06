//
// Created by Shawn on 2022/4/5.
//
#pragma once

#include <OpenGL/Mesh.h>
#include <AssetDatabase/AssetDatabase.h>
#include <imgui.h>
#include <IconsKenney.h>
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

		/**
 		* return the private _mesh parameter
 		* @return
 		*/
		 Mesh* GetMesh();

	private:
		Mesh* _mesh = nullptr;
		AssetDatabase _assetDatabase;
		Asset _asset;
	};
}