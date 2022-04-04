//
// Created by shawn on 2022/3/7.
//

#include "WindowManager/Filedialog.h"

namespace PlatinumEngine
{
	const ImVec2 FileDialog::MIN_SIZE = ImVec2((float)540, (float)480);
	const ImVec2 FileDialog::MAX_SIZE = ImVec2((float)1080, (float)960);
	std::string FileDialog::SaveFile()
	{
		std::string filePath;

		if(ImGuiFileDialog::Instance()->Display("SaveFileKey", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking, MIN_SIZE, MAX_SIZE))
		{
			if(ImGuiFileDialog::Instance()->IsOk())
			{
				filePath = ImGuiFileDialog ::Instance()->GetCurrentPath();
			}

			ImGuiFileDialog::Instance()->Close();
		}
		return filePath;
	}

	std::string FileDialog::LoadFile()
	{
		std::string filePathName;

		if(ImGuiFileDialog::Instance()->Display("LoadFileKey", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking, MIN_SIZE, MAX_SIZE))
		{
			if(ImGuiFileDialog::Instance()->IsOk())
			{
				filePathName = ImGuiFileDialog ::Instance()->GetFilePathName();
			}
			ImGuiFileDialog::Instance()->Close();
		}
		return filePathName;
	}
}