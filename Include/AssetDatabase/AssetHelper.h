//
// Created by Shawn on 2022/4/5.
//
#pragma once

#include <OpenGL/Mesh.h>
#include <OpenGL/Texture.h>
#include <AssetDatabase/AssetDatabase.h>
#include <IDSystem/IDSystem.h>
#include <imgui.h>
#include <IconsFontAwesome6.h>
#include <tuple>

namespace PlatinumEngine
{
	class AssetHelper
	{
	public:
		AssetHelper(AssetDatabase* assetDatabase, IDSystem& idSystem);

		/**
		 * Shows an ImGui popup window with every asset loaded in with the select type T.
		 * @tparam T type to show in popup list
		 * @param popupText text to show in popup, must be unique
		 * @return {true, something} if user clicked some thing, else {false, null}
		 */
		template<typename T>
		std::tuple<bool, SavedReference<T>> PickAssetGUIWindow(const char* popupText)
		{
			bool isAssetSelected = false;
			SavedReference<T> assetReference;
			static ImGuiTextFilter filter;

			if (ImGui::BeginPopupModal(popupText, nullptr,
					ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
			{
				filter.Draw(ICON_FA_MAGNIFYING_GLASS);
				ImGui::Separator();
				if (ImGui::Selectable("None"))
				{
					isAssetSelected = true;
				}
				for (AssetWithType<T>& assetWithType: _assetDatabase->GetAssets<T>())
				{
					std::string assetPath = assetWithType.asset.path.string();
					if (filter.PassFilter(assetPath.c_str()))
					{
						if (ImGui::Selectable(assetPath.c_str()))
						{
							isAssetSelected = true;
							assetReference = std::move(assetWithType.GetSavedReference(_idSystem));
						}
					}
				}
				ImGui::EndPopup();
			}
			return { isAssetSelected, assetReference };
		}

	private:
		AssetDatabase* _assetDatabase;
		IDSystem& _idSystem;
	};
}