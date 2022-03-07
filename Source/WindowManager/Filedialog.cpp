//
// Created by shawn on 2022/3/7.
//

#include "WindowManager/Filedialog.h"

namespace PlatinumEngine
{
	ImVec2 maxSize = ImVec2((float)1080, (float)960);
	ImVec2 minSize = ImVec2((float)540, (float)480);

	std::string fileDialog::SaveFile()
	{
		std::string filePath;

		if(ImGuiFileDialog::Instance()->Display("SaveFileKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
		{
			if(ImGuiFileDialog::Instance()->IsOk())
			{
				filePath = ImGuiFileDialog ::Instance()->GetCurrentPath();
			}

			ImGuiFileDialog::Instance()->Close();
		}
		return filePath;
	}

	std::string fileDialog::LoadFile()
	{
		std::string filePathName;

		if(ImGuiFileDialog::Instance()->Display("LoadFileKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
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