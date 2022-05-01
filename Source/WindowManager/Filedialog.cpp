//
// Created by shawn on 2022/3/7.
//

#include <WindowManager/Filedialog.h>
#include <Logger/Logger.h>

namespace PlatinumEngine::FileDialog
{
	constexpr ImVec2 MIN_SIZE = ImVec2((float)540, (float)480);
	constexpr ImVec2 MAX_SIZE = ImVec2((float)1080, (float)960);

	std::pair<bool, std::string> OpenDialog(
			bool* outIsOpen,
			const std::string& keyName,
			const std::string& title,
			const char* filters,
			const std::string& path,
			const std::string& fileName,
			const int& countSelectionMax,
			ImGuiFileDialogFlags flags)
	{
		ImGuiFileDialog::Instance()->OpenDialog(
				keyName,
				title,
				filters,
				path,
				fileName,
				countSelectionMax,
				nullptr,
				flags);

		std::pair<bool, std::string> output;

		if (ImGuiFileDialog::Instance()->Display(keyName, ImGuiWindowFlags_NoCollapse, MIN_SIZE, MAX_SIZE))
		{
			*outIsOpen = false;

			if (ImGuiFileDialog::Instance()->IsOk())
			{
				output = { true, ImGuiFileDialog::Instance()->GetFilePathName() };
			}

			ImGuiFileDialog::Instance()->Close();
		}

		return output;
	}
}