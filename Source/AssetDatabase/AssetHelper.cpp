#include "AssetDatabase/AssetHelper.h"
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	AssetHelper::AssetHelper(AssetDatabase* assetDatabase):_assetDatabase(assetDatabase)
	{
		_assetDatabase->Update();
	}

	AssetHelper::~AssetHelper() {}

	std::tuple<bool, Mesh*> AssetHelper::ShowMeshGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Mesh* mesh;
		if(ImGui::BeginPopupModal("Select Mesh", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			filter.Draw(ICON_FA_MAGNIFYING_GLASS);
			ImGui::Separator();
			if(ImGui::Selectable("None"))
			{
				mesh = nullptr;
				isAssetSelected= true;
			}
			for(auto meshAssetID : _assetDatabase->GetMeshAssetIDs())
			{
				if(_assetDatabase->GetAsset(meshAssetID.id, asset))
				{
					if (filter.PassFilter(asset.path.string().c_str()))
					{
						if (ImGui::Selectable(asset.path.string().c_str()))
						{
							mesh = (*_assetDatabase)[meshAssetID];
							isAssetSelected = true;
						}
					}
				}
				else
				{
					PLATINUM_ERROR("NO SUCH ASSET FOUND IN ASSET DATABASE!");
				}
			}
			ImGui::EndPopup();
		}
		return {isAssetSelected, mesh};
	}

	// TODO: This is unDone
	std::tuple<bool, Texture*> AssetHelper::ShowTextureGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Texture* texture;

		if(ImGui::BeginPopupModal("Select Texture", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			filter.Draw(ICON_FA_MAGNIFYING_GLASS);
			ImGui::Separator();
			if(ImGui::Selectable("None"))
			{
				texture = nullptr;
				isAssetSelected= true;
			}
			for(auto textureAssetID : _assetDatabase->GetTextureAssetIDs())
			{
				if(_assetDatabase->GetAsset(textureAssetID.id, asset))
				{
					if (filter.PassFilter(asset.path.string().c_str()))
					{
						if (ImGui::Selectable(asset.path.string().c_str()))
						{
							texture= (*_assetDatabase)[textureAssetID];
							isAssetSelected = true;
						}
					}
				}
				else
				{
					PLATINUM_ERROR("NO SUCH ASSET FOUND IN ASSET DATABASE!");
				}
			}
			ImGui::EndPopup();
		}
		return {isAssetSelected, texture};
	}

	// TODO: This is an exact copy of function above so we can select a normal texture, this is bad
	std::tuple<bool, Texture*> AssetHelper::ShowNormalTextureGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Texture* texture;

		if(ImGui::BeginPopupModal("Select Normal Texture", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			filter.Draw(ICON_FA_MAGNIFYING_GLASS);
			ImGui::Separator();
			if(ImGui::Selectable("None"))
			{
				texture = nullptr;
				isAssetSelected= true;
			}
			for(auto textureAssetID : _assetDatabase->GetTextureAssetIDs())
			{
				if(_assetDatabase->GetAsset(textureAssetID.id, asset))
				{
					if (filter.PassFilter(asset.path.string().c_str()))
					{
						if (ImGui::Selectable(asset.path.string().c_str()))
						{
							texture= (*_assetDatabase)[textureAssetID];
							isAssetSelected = true;
						}
					}
				}
				else
				{
					PLATINUM_ERROR("NO SUCH ASSET FOUND IN ASSET DATABASE!");
				}
			}
			ImGui::EndPopup();
		}
		return {isAssetSelected, texture};
	}
}
