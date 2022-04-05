//
// Created by Matt on 05/04/2022.
//

#pragma once

#include <imgui.h>

namespace PlatinumEngine
{
	class ProjectWindow
	{
	public:
		ProjectWindow() = default;
		void ShowGUIWindow(bool* isOpen);
	};
}