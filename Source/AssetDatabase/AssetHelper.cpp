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
				if(_assetDatabase->TryGetAsset(meshAssetID.id, asset))
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
	//TODO: Maybe a more efficient procedure to utilise TryGetAsset using filePath
	std::tuple<bool, Mesh*> AssetHelper::GetMeshAsset(std::string filePath)
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Mesh* mesh = nullptr;
		for(auto meshAssetID : _assetDatabase->GetMeshAssetIDs())
		{
			if (_assetDatabase->TryGetAsset(meshAssetID.id, asset))
			{
				if(asset.path.compare(std::filesystem::path(filePath))==0)
				{
					mesh = (*_assetDatabase)[meshAssetID];
					isAssetSelected = true;
				}
			}
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
				if(_assetDatabase->TryGetAsset(textureAssetID.id, asset))
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

	//TODO: Maybe a more efficient procedure to utilise TryGetAsset using filePath
	std::tuple<bool, Texture*> AssetHelper::GetTextureAsset(std::string filePath)
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Texture* texture = nullptr;
		for(auto textureAssetID : _assetDatabase->GetTextureAssetIDs())
		{
			if (_assetDatabase->TryGetAsset(textureAssetID.id, asset))
			{
				if(asset.path.compare(std::filesystem::path(filePath))==0)
				{
					isAssetSelected = true;
					texture = (*_assetDatabase)[textureAssetID];
				}
			}
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
				if(_assetDatabase->TryGetAsset(textureAssetID.id, asset))
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

	std::tuple<bool, Texture*> AssetHelper::ShowGeneralTextureGuiWindow(const char *popUpName)
	{
		bool isAssetSelected = false;
		static ImGuiTextFilter filter;
		Asset asset;
		Texture* texture;

		if(ImGui::BeginPopupModal(popUpName, nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
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
				if(_assetDatabase->TryGetAsset(textureAssetID.id, asset))
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
