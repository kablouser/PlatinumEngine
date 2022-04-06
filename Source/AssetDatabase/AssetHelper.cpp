#include "AssetDatabase/AssetHelper.h"

namespace PlatinumEngine
{
	AssetHelper::AssetHelper():_asset(), _assetDatabase()
	{
		_assetDatabase.Update();
	}

	AssetHelper::~AssetHelper() {}

	bool AssetHelper::ShowGuiWindow()
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		if(ImGui::BeginPopupModal("Select Mesh", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			filter.Draw(ICON_KI_SEARCH);
			std::vector<std::string> filePath;
			ImGui::Separator();
			for(auto meshAssetID : _assetDatabase.GetMeshAssetIDs())
			{
				_assetDatabase.TryGetAsset(meshAssetID.id, _asset);

				if(filter.PassFilter(_asset.path.c_str()))
				{
					if(ImGui::Selectable(_asset.path.c_str()))
					{
						_mesh = _assetDatabase[meshAssetID];
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
}
