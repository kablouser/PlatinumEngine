//
// Created by Shawn on 2022/4/5.
//
#pragma once

#include <OpenGL/Mesh.h>
#include <OpenGL/Texture.h>
#include <AssetDatabase/AssetDatabase.h>
#include <imgui.h>
#include <IconsFontAwesome6.h>
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

		 std::tuple<bool, Mesh*> GetMeshAsset(std::string filePath);

		 std::tuple<bool, Texture*> ShowTextureGuiWindow();

		 std::tuple<bool, Texture*> GetTextureAsset(std::string filePath);

		 std::tuple<bool, Texture*> ShowNormalTextureGuiWindow();
		 
		 std::tuple<bool, std::string> ShowAudioGuiWindow();

		std::tuple<bool, std::string> GetAudioAsset(std::string filePath);

	private:
		AssetDatabase* _assetDatabase;
	};
}