//
// Created by Shihua on 02/03/2022.
//

#include "SceneEditor/SceneEditor.h"
#include <imgui.h>
//#include <imgui-SFML.h>


using namespace PlatinumEngine;

SceneEditor::SceneEditor(
		unsigned int depthBits,
		unsigned int stencilBits,
		unsigned int antiAliasingLevel)
{

	// use a random number instead
//	_renderTexture.create(1.f,1.f,_contextSettings);

}

void SceneEditor::ShowGUIWindow(bool* outIsOpen, Renderer& rasterRenderer)
{

	// begin the ImGui window
	if(ImGui::Begin("Raster Renderer", outIsOpen))
	{
		// get window size
		auto targetSize = ImGui::GetContentRegionAvail();

		// get rendering result from Renderer
//		rasterRenderer.RenderObjects(_renderTexture, targetSize);

		// put target into GUI window
//		ImGui::Image(_renderTexture, targetSize);
	}
	ImGui::End();
}






