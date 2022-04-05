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

		if(ImGui::BeginPopupModal("Select Mesh", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputText("##Search", _meshBuffer, sizeof(_meshBuffer));
			ImGui::Separator();
			for(auto meshAssetID : _assetDatabase.GetMeshAssetIDs())
			{
				_assetDatabase.TryGetAsset(meshAssetID.id, _asset);
				if(ImGui::Selectable(_asset.path.c_str()))
				{
					_mesh = _assetDatabase[meshAssetID];
					isAssetSelected= true;
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
