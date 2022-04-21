//
// Created by Jinyuan and Shawn on 26/03/2022.
//

#pragma once
#include "OpenGL/Mesh.h"
#include "Component.h"
#include "Renderer/Renderer.h"
#include "Loaders/MeshLoader.h"
#include <AssetDatabase/AssetHelper.h>
#include "OpenGL/Material.h"
namespace PlatinumEngine
{
	class RenderComponent: public Component
	{
	public:
		RenderComponent();

		// update mesh from mesh loader
		void SetMesh(Mesh* mesh);

		void SetMaterial(Texture* texture);

		// return mesh
		Mesh* GetMesh();

		// override the OnRender() of the Component
		void OnRender(Scene& scene, Renderer& renderer) override;

	public:
		Material material;

	private:
		Mesh* _mesh;


		ShaderInput _shaderInput;
	};
}
