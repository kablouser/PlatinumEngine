#include "AssetDatabase/AssetHelper.h"
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	AssetHelper::AssetHelper(AssetDatabase* assetDatabase):_assetDatabase(assetDatabase)
	{
		_assetDatabase->Update();
	}

	AssetHelper::~AssetHelper() {}

	std::tuple<bool, Mesh*> AssetHelper::ShowGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Mesh* mesh;
		if(ImGui::BeginPopupModal("Select Mesh", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			filter.Draw(ICON_KI_SEARCH);
			ImGui::Separator();
			if(ImGui::Selectable("None"))
			{
				mesh = nullptr;
				isAssetSelected= true;
			}
			for(auto meshAssetID : _assetDatabase->GetMeshAssetIDs())
			{
				if(_assetDatabase->TryGetAsset(meshAssetID.id, asset))
				{
					if (filter.PassFilter(asset.path.string().c_str()))
					{
						if (ImGui::Selectable(asset.path.string().c_str()))
						{
							mesh = (*_assetDatabase)[meshAssetID];
							mesh->fileName = asset.path.filename().string();
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
}
