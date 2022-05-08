#include "AssetDatabase/AssetHelper.h"
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	AssetHelper::AssetHelper(AssetDatabase* assetDatabase, IDSystem& idSystem) :
			_assetDatabase(assetDatabase),
			_idSystem(idSystem)
	{
	}
	
	std::tuple<bool, std::string> AssetHelper::ShowAudioGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		std::string audioSample;
		if(ImGui::BeginPopupModal("Select Sample", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			//filter.Draw(ICON_FA_MAGNIFYING_GLASS);
			ImGui::Separator();
			if(ImGui::Selectable("None"))
			{
				audioSample = "";
				isAssetSelected= true;
			}
			for(auto audioAssetID : _assetDatabase->GetAudioAssetIDs())
			{
				if(_assetDatabase->TryGetAsset(audioAssetID.id, asset))
				{
					if (filter.PassFilter(asset.path.string().c_str()))
					{
						if (ImGui::Selectable(asset.path.string().c_str()))
						{
							audioSample = (*_assetDatabase)[audioAssetID];
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
		return {isAssetSelected, audioSample};
	}

	std::tuple<bool, std::string> AssetHelper::GetAudioAsset(std::string filePath)
	{
		bool isAssetSelected = true;
		static ImGuiTextFilter filter;
		Asset asset;
		std::string sample = "";
		for(auto audioAssetID : _assetDatabase->GetAudioAssetIDs())
		{
			if (_assetDatabase->TryGetAsset(audioAssetID.id, asset))
			{
				if(asset.path.compare(std::filesystem::path(filePath))==0)
					sample = (*_assetDatabase)[audioAssetID];
			}
		}
		return {isAssetSelected, sample};
	}

}
