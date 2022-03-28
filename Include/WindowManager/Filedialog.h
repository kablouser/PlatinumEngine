#pragma once
#include <ImGuiFileDialog.h>

namespace PlatinumEngine
{
	class FileDialog
	{
	public:
		static const ImVec2 MIN_SIZE;
		static const ImVec2 MAX_SIZE;
		static std::string SaveFile();
		static std::string LoadFile();
	};
}