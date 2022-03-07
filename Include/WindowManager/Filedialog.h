#pragma once
#include <ImGuiFileDialog.h>

namespace PlatinumEngine
{
	class fileDialog
	{
	public:
		static std::string SaveFile();
		static std::string LoadFile();
	};
}