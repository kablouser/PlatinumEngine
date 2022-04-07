#include "AssetDatabase/AssetHelper.h"

namespace PlatinumEngine
{
	AssetHelper::AssetHelper(AssetDatabase* assetDatabase):_assetDatabase(assetDatabase)
	{
		_assetDatabase->Update();
	}

	AssetHelper::~AssetHelper() {}

	std::tuple<bool, Mesh*, std::string> AssetHelper::ShowGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Mesh* mesh;
		std::string returnFilePath;
		if(ImGui::BeginPopupModal("Select Mesh", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			filter.Draw(ICON_KI_SEARCH);
			ImGui::Separator();
			if(ImGui::Selectable("None"))
			{
				mesh= nullptr;
				isAssetSelected= true;
				returnFilePath = "";
			}
			for(auto meshAssetID : _assetDatabase->GetMeshAssetIDs())
			{
				_assetDatabase->TryGetAsset(meshAssetID.id, asset);
				if(filter.PassFilter(asset.path.string().c_str()))
				{
					if(ImGui::Selectable(asset.path.string().c_str()))
					{
						mesh = (*_assetDatabase)[meshAssetID];
						returnFilePath = asset.path.filename().string();
						isAssetSelected= true;
					}
				}
			}
			ImGui::EndPopup();
		}
		return {isAssetSelected, mesh, returnFilePath};
	}
}
