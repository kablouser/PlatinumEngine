//
// Created by Jinyuan and Shawn on 26/03/2022.
//

#pragma once
#include "OpenGL/Mesh.h"
#include "Component.h"
#include "Renderer/Renderer.h"
#include "Loaders/MeshLoader.h"
#include <IDSystem/IDSystem.h>
#include "OpenGL/Material.h"
namespace PlatinumEngine
{
	class MeshRender: public Component
	{
	public:

		static void CreateTypeInfo(TypeDatabase& database);

		MeshRender();

		// update mesh from mesh loader
		void SetMesh(SavedReference<Mesh> mesh);
		void SetMaterial(SavedReference<Texture> texture);

		void SetNormalMap(SavedReference<Texture> texture);

		// return mesh
		SavedReference<Mesh>& GetMesh();

		// override the OnRender() of the Component
		void OnRender(Scene& scene, Renderer& renderer) override;

		void OnIDSystemUpdate(Scene& scene) override;

		Material material;


	private:
		SavedReference<Mesh> _mesh;
		ShaderInput _shaderInput;
	};
}
