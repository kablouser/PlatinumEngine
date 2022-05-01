//
// Created by Jinyuan on 01/04/2022.
//

#include "ComponentComposition/Objects.h"
namespace PlatinumEngine
{

	Mesh ArrowMesh(float rbase, float rtip, float height)
	{
		MeshData base = Cone(rbase, rbase, 0.75f * height, 10, true);
		MeshData tip = Cone(rtip, 0.001f, 0.25f * height, 10, true);
		for(auto& v : tip.vertices) v.position.y += 0.7f;
		return Merge(std::move(base), std::move(tip));
	}

	Mesh CubeMesh(float xSize, float ySize, float zSize)
	{
		MeshData cubeData = Cube(xSize, ySize, zSize);
		Mesh cubeMesh = Mesh(cubeData.vertices, cubeData.indices);
		return cubeMesh;
	}


	Mesh SphereMesh(float r, int subdivsions)
	{
		auto ico_sphere = IconSphere(r, subdivsions);
		return {std::move(ico_sphere.vertices), std::move(ico_sphere.indices)};
	}

	Mesh ConeMesh(float bradius, float tradius, float height, int sides, bool cap)
	{
		MeshData cone = Cone(bradius, tradius, height, sides, cap);
		return {std::move(cone.vertices), std::move(cone.indices)};
	}

	Mesh CapsuleMesh(float h, float r)
	{
		MeshData bottom = uv_hemisphere(r);
		MeshData top = uv_hemisphere(r);
		for(auto& v : top.vertices) v.position.y = -v.position.y + h;
		MeshData cyl = Cone(r, r, h, 64, false);

		auto cyl_off = (unsigned int)bottom.vertices.size();
		auto top_off = cyl_off + (unsigned int)cyl.vertices.size();

		for(auto& i : cyl.indices) i += cyl_off;
		for(auto& i : top.indices) i += top_off;

		bottom.vertices.insert(bottom.vertices.end(), cyl.vertices.begin(), cyl.vertices.end());
		bottom.indices.insert(bottom.indices.end(), cyl.indices.begin(), cyl.indices.end());

		bottom.vertices.insert(bottom.vertices.end(), top.vertices.begin(), top.vertices.end());
		bottom.indices.insert(bottom.indices.end(), top.indices.begin(), top.indices.end());

		return {std::move(bottom.vertices), std::move(bottom.indices)};
	}

	MeshData Cube(float xSize, float ySize, float zSize)
	{
		return {{{{Maths::Vec3(-xSize, -ySize, -zSize)}, {Maths::Vec3(-xSize, -ySize, -zSize)}, {Maths::Vec2(0.0, 0.0)}},
						{{Maths::Vec3(xSize, -ySize, -zSize)}, {Maths::Vec3(xSize, -ySize, -zSize)}, {Maths::Vec2(0.0, 0.0)}},
						{{ Maths::Vec3(xSize, ySize, -zSize) }, { Maths::Vec3(xSize, ySize, -zSize) }, { Maths::Vec2(0.0, 0.0) }},
						{{ Maths::Vec3(-xSize, ySize, -zSize) }, { Maths::Vec3(-xSize, ySize, -zSize) },  { Maths::Vec2(0.0, 0.0) }},
						{{ Maths::Vec3(-xSize, -ySize, zSize) }, { Maths::Vec3(-xSize, -ySize, zSize) },  { Maths::Vec2(0.0, 0.0) }},
						{{ Maths::Vec3(xSize, -ySize, zSize) }, { Maths::Vec3(xSize, -ySize, zSize) },   { Maths::Vec2(0.0, 0.0) }},
						{{ Maths::Vec3(xSize, ySize, zSize) }, { Maths::Vec3(xSize, ySize, zSize) },    { Maths::Vec2(0.0, 0.0) }},
						{{ Maths::Vec3(-xSize, ySize, zSize) }, { Maths::Vec3(-xSize, ySize, zSize) },   { Maths::Vec2(0.0, 0.0) }}},
				{0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7,
						4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1}};
	}

	MeshData Cone(float bradius, float tradius, float height, int sides, bool caps)
	{
		const size_t n_sides = sides, n_cap = n_sides + 1;
		const float _2pi = PI_F * 2.0f;

		std::vector<Maths::Vec3> vertices(n_cap + n_cap + n_sides * 2 + 2);
		size_t vert = 0;

		vertices[vert++] = Maths::Vec3(0.0f, 0.0f, 0.0f);

		float t = 0.0f;
		float step = _2pi / n_sides;

		while(vert <= n_sides) {
			vertices[vert] = Maths::Vec3(std::cos(t) * bradius, 0.0f, std::sin(t) * bradius);
			vert++;
			t += step;
		}

		vertices[vert++] = Maths::Vec3(0.0f, height, 0.0f);
		t = 0.0f;
		while(vert <= n_sides * 2 + 1) {
			vertices[vert] = Maths::Vec3(std::cos(t) * tradius, height, std::sin(t) * tradius);
			vert++;
			t += step;
		}

		size_t v = 0;
		t = 0.0f;
		while(vert <= vertices.size() - 4) {
			vertices[vert] = Maths::Vec3(std::cos(t) * tradius, height, std::sin(t) * tradius);
			vertices[vert + 1] = Maths::Vec3(std::cos(t) * bradius, 0.0f, std::sin(t) * bradius);
			vert += 2;
			v++;
			t += step;
		}
		vertices[vert] = vertices[n_sides * 2 + 2];
		vertices[vert + 1] = vertices[n_sides * 2 + 3];

		std::vector<Maths::Vec3> normals(vertices.size());
		vert = 0;
		while(vert <= n_sides) {
			normals[vert++] = Maths::Vec3(0.0f, -1.0f, 0.0f);
		}
		while(vert <= n_sides * 2 + 1) {
			normals[vert++] = Maths::Vec3(0.0f, 1.0f, 0.0f);
		}

		v = 0;
		while(vert <= vertices.size() - 4) {
			float rad = (float)v / n_sides * _2pi;
			float cos = std::cos(rad);
			float sin = std::sin(rad);
			normals[vert] = Maths::Vec3(cos, 0.0f, sin);
			normals[vert + 1] = normals[vert];
			vert += 2;
			v++;
		}
		normals[vert] = normals[n_sides * 2 + 2];
		normals[vert + 1] = normals[n_sides * 2 + 3];

		size_t n_tris = n_sides + n_sides + n_sides * 2;
		std::vector<unsigned int> triangles(n_tris * 3 + 3);

		unsigned int tri = 0;
		size_t i = 0;
		while(tri < n_sides - 1) {
			if(caps) {
				triangles[i] = 0;
				triangles[i + 1] = tri + 1;
				triangles[i + 2] = tri + 2;
			}
			tri++;
			i += 3;
		}
		if(caps) {
			triangles[i] = 0;
			triangles[i + 1] = tri + 1;
			triangles[i + 2] = 1;
		}
		tri++;
		i += 3;

		while(tri < n_sides * 2) {
			if(caps) {
				triangles[i] = tri + 2;
				triangles[i + 1] = tri + 1;
				triangles[i + 2] = (GLuint)n_cap;
			}
			tri++;
			i += 3;
		}
		if(caps) {
			triangles[i] = (GLuint)n_cap + 1;
			triangles[i + 1] = tri + 1;
			triangles[i + 2] = (GLuint)n_cap;
		}
		tri++;
		i += 3;
		tri++;

		while(tri <= n_tris) {
			triangles[i] = tri + 2;
			triangles[i + 1] = tri + 1;
			triangles[i + 2] = tri + 0;
			tri++;
			i += 3;
			triangles[i] = tri + 1;
			triangles[i + 1] = tri + 2;
			triangles[i + 2] = tri + 0;
			tri++;
			i += 3;
		}

		std::vector<Vertex> verts;
		for(size_t j = 0; j < vertices.size(); j++) {
			verts.push_back({vertices[j], normals[j], Maths::Vec2(0.0, 0.0)});
		}
		return {verts, triangles};
	}

	Mesh QuadMesh(float x, float y)
	{
		MeshData square = Quad(x, y);
		return {std::move(square.vertices), std::move(square.indices)};
	}
	Mesh SquareMesh(float r)
	{
		MeshData square = Quad(r, r);
		return {std::move(square.vertices), std::move(square.indices)};
	}

	MeshData uv_hemisphere(float radius)
	{
		int nbLong = 64;
		int nbLat = 16;

		std::vector<Maths::Vec3> vertices((nbLong + 1) * nbLat + 2);
		float _pi = PI_F;
		float _2pi = _pi * 2.0f;

		vertices[0] = Maths::Vec3{0.0f, radius, 0.0f};
		for(int lat = 0; lat < nbLat; lat++) {
			float a1 = _pi * (float)(lat + 1) / (nbLat + 1);
			float sin1 = std::sin(a1);
			float cos1 = std::cos(a1);

			for(int lon = 0; lon <= nbLong; lon++) {
				float a2 = _2pi * (float)(lon == nbLong ? 0 : lon) / nbLong;
				float sin2 = std::sin(a2);
				float cos2 = std::cos(a2);

				vertices[lon + lat * (nbLong + 1) + 1] = Maths::Vec3(sin1 * cos2, cos1, sin1 * sin2) * radius;
			}
		}
		vertices[vertices.size() - 1] = Maths::Vec3{0.0f, -radius, 0.0f};

		std::vector<Maths::Vec3> normals(vertices.size());
		for(size_t n = 0; n < vertices.size(); n++) normals[n] = vertices[n].unit();

		int nbFaces = (int)vertices.size();
		int nbTriangles = nbFaces * 2;
		int nbIndexes = nbTriangles * 3;
		std::vector<unsigned int> triangles(nbIndexes);

		int i = 0;
		for(int lat = (nbLat - 1) / 2; lat < nbLat - 1; lat++) {
			for(int lon = 0; lon < nbLong; lon++) {
				int current = lon + lat * (nbLong + 1) + 1;
				int next = current + nbLong + 1;

				triangles[i++] = current;
				triangles[i++] = current + 1;
				triangles[i++] = next + 1;

				triangles[i++] = current;
				triangles[i++] = next + 1;
				triangles[i++] = next;
			}
		}

		for(int lon = 0; lon < nbLong; lon++) {
			triangles[i++] = (int)vertices.size() - 1;
			triangles[i++] = (int)vertices.size() - (lon + 2) - 1;
			triangles[i++] = (int)vertices.size() - (lon + 1) - 1;
		}

		std::vector<Vertex> verts;
		for(size_t j = 0; j < vertices.size(); j++) {
			verts.push_back({vertices[j], normals[j], Maths::Vec2(0, 0)});
		}
		triangles.resize(i);
		return {verts, triangles};
	}
	MeshData IconSphere(float radius, int level)
	{
		struct TriIdx {
			int v1, v2, v3;
		};

		auto middle_point = [&](int p1, int p2, std::vector<Maths::Vec3>& vertices,
				std::map<int64_t, size_t>& cache, float radius) -> size_t {
			bool firstIsSmaller = p1 < p2;
			int64_t smallerIndex = firstIsSmaller ? p1 : p2;
			int64_t greaterIndex = firstIsSmaller ? p2 : p1;
			int64_t key = (smallerIndex << 32ll) + greaterIndex;

			auto entry = cache.find(key);
			if(entry != cache.end()) {
				return entry->second;
			}

			Maths::Vec3 point1 = vertices[p1];
			Maths::Vec3 point2 = vertices[p2];
			Maths::Vec3 middle((point1.x + point2.x) / 2.0f, (point1.y + point2.y) / 2.0f,
					(point1.z + point2.z) / 2.0f);
			size_t i = vertices.size();
			vertices.push_back(middle.unit() * radius);
			cache[key] = i;
			return i;
		};

		std::vector<Maths::Vec3> vertices;
		std::map<int64_t, size_t> middlePointIndexCache;
		float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
		vertices.push_back(Maths::Vec3(-1.0f, t, 0.0f).unit() * radius);
		vertices.push_back(Maths::Vec3(1.0f, t, 0.0f).unit() * radius);
		vertices.push_back(Maths::Vec3(-1.0f, -t, 0.0f).unit() * radius);
		vertices.push_back(Maths::Vec3(1.0f, -t, 0.0f).unit() * radius);
		vertices.push_back(Maths::Vec3(0.0f, -1.0f, t).unit() * radius);
		vertices.push_back(Maths::Vec3(0.0f, 1.0f, t).unit() * radius);
		vertices.push_back(Maths::Vec3(0.0f, -1.0f, -t).unit() * radius);
		vertices.push_back(Maths::Vec3(0.0f, 1.0f, -t).unit() * radius);
		vertices.push_back(Maths::Vec3(t, 0.0f, -1.0f).unit() * radius);
		vertices.push_back(Maths::Vec3(t, 0.0f, 1.0f).unit() * radius);
		vertices.push_back(Maths::Vec3(-t, 0.0f, -1.0f).unit() * radius);
		vertices.push_back(Maths::Vec3(-t, 0.0f, 1.0f).unit() * radius);

		std::vector<TriIdx> faces;
		faces.push_back(TriIdx{0, 11, 5});
		faces.push_back(TriIdx{0, 5, 1});
		faces.push_back(TriIdx{0, 1, 7});
		faces.push_back(TriIdx{0, 7, 10});
		faces.push_back(TriIdx{0, 10, 11});
		faces.push_back(TriIdx{1, 5, 9});
		faces.push_back(TriIdx{5, 11, 4});
		faces.push_back(TriIdx{11, 10, 2});
		faces.push_back(TriIdx{10, 7, 6});
		faces.push_back(TriIdx{7, 1, 8});
		faces.push_back(TriIdx{3, 9, 4});
		faces.push_back(TriIdx{3, 4, 2});
		faces.push_back(TriIdx{3, 2, 6});
		faces.push_back(TriIdx{3, 6, 8});
		faces.push_back(TriIdx{3, 8, 9});
		faces.push_back(TriIdx{4, 9, 5});
		faces.push_back(TriIdx{2, 4, 11});
		faces.push_back(TriIdx{6, 2, 10});
		faces.push_back(TriIdx{8, 6, 7});
		faces.push_back(TriIdx{9, 8, 1});

		for(int i = 0; i < level; i++) {
			std::vector<TriIdx> faces2;
			for(auto tri : faces) {
				int a = (int)middle_point(tri.v1, tri.v2, vertices, middlePointIndexCache, radius);
				int b = (int)middle_point(tri.v2, tri.v3, vertices, middlePointIndexCache, radius);
				int c = (int)middle_point(tri.v3, tri.v1, vertices, middlePointIndexCache, radius);
				faces2.push_back(TriIdx{tri.v1, a, c});
				faces2.push_back(TriIdx{tri.v2, b, a});
				faces2.push_back(TriIdx{tri.v3, c, b});
				faces2.push_back(TriIdx{a, b, c});
			}
			faces = faces2;
		}

		std::vector<unsigned int> triangles;
		for(size_t i = 0; i < faces.size(); i++) {
			triangles.push_back(faces[i].v1);
			triangles.push_back(faces[i].v2);
			triangles.push_back(faces[i].v3);
		}

		std::vector<Maths::Vec3> normals(vertices.size());
		for(size_t i = 0; i < normals.size(); i++) normals[i] = vertices[i].unit();

		std::vector<Vertex> verts;
		for(size_t i = 0; i < vertices.size(); i++) {
			verts.push_back({vertices[i], normals[i], { 0.0, 0.0 }});
		}
		return {verts, triangles};
	}

	MeshData Quad(float x, float y)
	{
		return {{{Maths::Vec3{-x, 0.0f, -y}, Maths::Vec3{0.0f, 1.0f, 0.0f}, { 0.0, 0.0 }},
				{Maths::Vec3{-x, 0.0f, y}, Maths::Vec3{0.0f, 1.0f, 0.0f}, { 0.0, 0.0 }},
				{Maths::Vec3{x, 0.0f, -y}, Maths::Vec3{0.0f, 1.0f, 0.0f}, { 0.0, 0.0 }},
				  {Maths::Vec3{x, 0.0f, y}, Maths::Vec3{0.0f, 1.0f, 0.0f}, { 0.0, 0.0 }}},
				{0, 1, 2, 2, 1, 3}};
	}

	Mesh Dedup(MeshData&& d)
	{
		std::vector<Vertex> verts;
		std::vector<unsigned int> elems;

		// normals be damned
		std::map<Maths::Vec3, unsigned int> v_to_idx;

		for(size_t i = 0; i < d.indices.size(); i++)
		{
			unsigned int idx = d.indices[i];
			Vertex v = d.vertices[idx];
			auto entry = v_to_idx.find(v.position);
			unsigned int new_idx;
			if(entry == v_to_idx.end()) {
				new_idx = (unsigned int)verts.size();
				v_to_idx.insert({v.position, new_idx});
				verts.push_back(v);
			} else {
				new_idx = entry->second;
			}
			elems.push_back(new_idx);
		}

		return {std::move(verts), std::move(elems)};
	}

	Mesh Merge(MeshData&& l, MeshData&& r)
	{
		for(auto& i : r.indices) i += (GLuint)l.vertices.size();
		l.vertices.insert(l.vertices.end(), r.vertices.begin(), r.vertices.end());
		l.indices.insert(l.indices.end(), r.indices.begin(), r.indices.end());
		return Mesh(std::move(l.vertices), std::move(l.indices));
	}
}