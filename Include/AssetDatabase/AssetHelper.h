//
// Created by Shawn on 2022/4/5.
//
#pragma once

#include <OpenGL/Mesh.h>
#include <OpenGL/Texture.h>
#include <AssetDatabase/AssetDatabase.h>
#include <imgui.h>
#include <IconsKenney.h>
#include <tuple>

namespace PlatinumEngine
{
	class AssetHelper
	{
	public:
		AssetHelper(AssetDatabase* assetDatabase);
		~AssetHelper();
		/**
		 * Responsible for the pop up window opened by
		 * the render component in Inspector Window
		 */
		 std::tuple<bool, Mesh*> ShowMeshGuiWindow();

		 std::tuple<bool, Texture*> ShowTextureGuiWindow();
	private:
		AssetDatabase* _assetDatabase;
	};
}