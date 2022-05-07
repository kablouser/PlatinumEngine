#pragma once

#include <ImGuiFileDialog.h>

namespace PlatinumEngine::FileDialog
{
	/**
	 * @param keyName unique identifier
	 * @param title
	 * @param filters
	 * @param path starting path when the dialog opens
	 * @param fileName default file name
	 * @param countSelectionMax
	 * @param flags
	 * @return {true, file path} if Ok clicked, {false, ""} otherwise
	 */
	std::pair<bool, std::string> OpenDialog(
			bool* outIsOpen,
			const std::string& keyName,
			const std::string& title,
			const char* filters,
			const std::string& path,
			const std::string& fileName,
			const int& countSelectionMax = 1,
			ImGuiFileDialogFlags flags = 0);
}