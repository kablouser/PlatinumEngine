//
// Created by Matt on 05/04/2022.
//

#include <Project/ProjectWindow.h>

using namespace PlatinumEngine;

void ProjectWindow::ShowGUIWindow(bool* isOpen)
{
	if(ImGui::Begin("Project Window", isOpen))
	{
		
	}

	ImGui::End();

}