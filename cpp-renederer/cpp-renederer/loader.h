#pragma once

#include<string>
#include<vector>

#include"vector3.h"
#include"vector2.h"
#include"materials.h"

// The .obj model loader
class Loader {
public:
	Loader();
	~Loader();
	bool loadFile(std::string path);

	std::vector<Mesh> loadedMeshes;
	std::vector<Vertex> loadedVertices;
	std::vector<unsigned int> loadedIndices;
	std::vector<Material> loadedMaterials;
private:
	void generateVertices(std::vector<Vertex>& oVerts, const std::vector<Vector3f>& iPositions,
		const std::vector<Vector2f>& iTCoords, const std::vector<Vector3f>& iNormals, std::string iCurline);
	void vertexTriangulation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts);
	bool loadMaterials(std::string path);
};
