//
// Created by Jinyuan on 01/04/2022.
//

#pragma once
#include <map>
#include <string>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include "OpenGL/Mesh.h"
#include "Maths/Vectors.h"

namespace PlatinumEngine
{
	//#define PI 3.14159265358979323846264338327950288f
	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};


	Mesh CubeMesh(float xSize, float ySize, float zSize);
	Mesh CapsuleMesh(float h, float r);
	Mesh SphereMesh(float r, int subdivsions);
	Mesh ConeMesh(float bradius, float tradius, float height, int sides = 12, bool cap = true);
	Mesh QuadMesh(float x, float y);
	Mesh SquareMesh(float r);

	MeshData Cube(float xSize, float ySize, float zSize);
	MeshData uv_hemisphere(float radius);
	MeshData Cone(float bradius, float tradius, float height, int sides, bool caps);
	MeshData IconSphere(float radius, int level);
	MeshData Quad(float x, float y);

	Mesh Dedup(MeshData&& d);
}

