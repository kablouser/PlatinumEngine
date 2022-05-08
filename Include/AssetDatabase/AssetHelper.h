//
// Created by Shawn on 2022/4/5.
//
#pragma once

#include <imgui.h>
#include <IconsFontAwesome6.h>
#include <tuple>
#include <Application.h>

namespace PlatinumEngine
{
	namespace AssetHelper
	{
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
				for (AssetWithType<T>& assetWithType: Application::Instance->assetDatabase.GetAssets<T>())
				{
					std::string assetPath = assetWithType.asset.path.string();
					if (filter.PassFilter(assetPath.c_str()))
					{
						if (ImGui::Selectable(assetPath.c_str()))
						{
							isAssetSelected = true;
							assetReference = std::move(assetWithType.GetSavedReference(Application::Instance->idSystem));
						}
					}
				}
				ImGui::EndPopup();
			}
			return { isAssetSelected, assetReference };
		}

		template<typename T>
		std::tuple<bool, SavedReference<T>> GetAsset(std::string filePath)
		{
			auto [success, asset] =	Application::Instance->assetDatabase.GetAsset(filePath);

			if (success)
				return {true ,Application::Instance->idSystem.GetSavedReference<T>(asset->id)};
			else
				return {false, {}};
		}
	};
}