//
// Created by Jinyuan and Shawn on 26/03/2022.
//

#pragma once
#include "OpenGL/Mesh.h"
#include "Component.h"
#include "Renderer/Renderer.h"
#include "Loaders/MeshLoader.h"
namespace PlatinumEngine
{
	class RenderComponent: public Component
	{
	public:
		RenderComponent();
		~RenderComponent();
		RenderComponent(Mesh &mesh);

		// update mesh from mesh loader
		void LoadMesh(const std::string &filePath);

		// return mesh
		Mesh& GetMesh();

		// override the OnRender() of the Component
		void OnRender(Renderer &render) override;
	private:
		Mesh _mesh;
	};
}
