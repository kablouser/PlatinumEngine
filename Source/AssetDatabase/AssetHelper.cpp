#include "AssetDatabase/AssetHelper.h"

namespace PlatinumEngine
{
	AssetHelper::AssetHelper():_assetDatabase()
	{
		_assetDatabase.Update();
	}

	AssetHelper::~AssetHelper() {}

	bool AssetHelper::ShowGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		if(ImGui::BeginPopupModal("Select Mesh", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			filter.Draw(ICON_KI_SEARCH);
			std::vector<std::string> filePath;
			ImGui::Separator();
			if(ImGui::Selectable("None"))
			{
				_mesh= nullptr;
				isAssetSelected= true;
				_filePath = "";
			}
			for(auto meshAssetID : _assetDatabase.GetMeshAssetIDs())
			{
				_assetDatabase.TryGetAsset(meshAssetID.id, asset);

				if(filter.PassFilter(asset.path.string().c_str()))
				{
					if(ImGui::Selectable(asset.path.string().c_str()))
					{
						_mesh = _assetDatabase[meshAssetID];
						_filePath = asset.path.string();
						isAssetSelected= true;
					}
				}
			}
			ImGui::EndPopup();
		}
		return isAssetSelected;
	}

	Mesh* AssetHelper::GetMesh()
	{
		return _mesh;
	}

	std::string AssetHelper::GetFilePath()
	{
		return _filePath;
	}
}
