#include<fstream>

#include"loader.h"
#include"loader_algorythms.h"

#define LOADER_CONSOLE_OUTPUT

Loader::Loader() {

}

Loader::~Loader()
{
	loadedMeshes.clear();
}

bool Loader::loadFile(std::string path)
{
	//	Return false if file is not an .obj file
	if (path.substr(path.size() - 4, 4) != ".obj")
		return false;

	std::ifstream file(path);

	//	Return false if failed to open file
	if (!file.is_open())
		return false;

	loadedMeshes.clear();
	loadedVertices.clear();
	loadedIndices.clear();

	std::vector<Vector3f> positions;
	std::vector<Vector2f> tCoords;
	std::vector<Vector3f> normals;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<std::string> meshMaterialNames;

	bool listening = false;
	std::string meshName;

	Mesh tempMesh;

	#ifdef LOADER_CONSOLE_OUTPUT
	const unsigned int outputEveryNth = 1000;
	unsigned int outputIndicator = outputEveryNth;
	#endif

	std::string currentLine;
	while (std::getline(file, currentLine))
	{
		#ifdef LOADER_CONSOLE_OUTPUT
			if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 1)
			{
				if (!meshName.empty())
				{
					std::cout
						<< "\r- " << meshName
						<< "\t| vertices > " << positions.size()
						<< "\t| texcoords > " << tCoords.size()
						<< "\t| normals > " << normals.size()
						<< "\t| triangles > " << (vertices.size() / 3)
						<< (!meshMaterialNames.empty() ? "\t| material: " + meshMaterialNames.back() : "");
				}
			}
		#endif

		//	Generate a mesh object
		if (firstToken(currentLine) == "o" || firstToken(currentLine) == "g" || currentLine[0] == 'g')
		{
			if (!listening)
			{
				listening = true;

				if (firstToken(currentLine) == "o" || firstToken(currentLine) == "g")
				{
					meshName = tail(currentLine);
				}
				else
				{
					meshName = "unnamed";
				}
			}
			else
			{
				// Generate the mesh

				if (!indices.empty() && !vertices.empty())
				{
					tempMesh = Mesh(vertices, indices);
					tempMesh.name = meshName;

					loadedMeshes.push_back(tempMesh);

					vertices.clear();
					indices.clear();
					meshName.clear();

					meshName = tail(currentLine);
				}
				else
				{
					if (firstToken(currentLine) == "o" || firstToken(currentLine) == "g")
					{
						meshName = tail(currentLine);
					}
					else
					{
						meshName = "unnamed";
					}
				}
			}
			#ifdef LOADER_CONSOLE_OUTPUT
			std::cout << std::endl;
			outputIndicator = 0;
			#endif
		}
		//	Generate vertex position
		if (firstToken(currentLine) == "v")
		{
			std::vector<std::string> sPosition;
			Vector3f vPosition;
			split(tail(currentLine), sPosition, " ");

			vPosition.x = std::stof(sPosition[0]);
			vPosition.y = std::stof(sPosition[1]);
			vPosition.z = std::stof(sPosition[2]);

			positions.push_back(vPosition);
		}

		//	Generate a Vertex texture coordinate
		if (firstToken(currentLine) == "vt")
		{
			std::vector<std::string> sTexture;
			Vector2f vtex;
			split(tail(currentLine), sTexture, " ");

			vtex.x = std::stof(sTexture[0]);
			vtex.y = std::stof(sTexture[1]);

			tCoords.push_back(vtex);
		}

		//	Generate a Vertex normal
		if (firstToken(currentLine) == "vn")
		{
			std::vector < std::string> sNormal;
			Vector3f vNormal;
			split(tail(currentLine), sNormal, " ");

			vNormal.x = std::stof(sNormal[0]);
			vNormal.y = std::stof(sNormal[1]);
			vNormal.z = std::stof(sNormal[2]);

			normals.push_back(vNormal);
		}

		//	Generate a face
		if (firstToken(currentLine) == "f")
		{
			std::vector<Vertex> vVerts;
			generateVertices(vVerts, positions, tCoords, normals, currentLine);

			// add vertices
			for (int i = 0; i < int(vVerts.size()); i++)
			{
				vertices.push_back(vVerts[i]);

				loadedVertices.push_back(vVerts[i]);
			}

			std::vector<unsigned int> iIndices;

			vertexTriangulation(iIndices, vVerts);

			for (int i = 0; i<int(iIndices.size()); i++)
			{
				unsigned int indexNumber = (unsigned int)((vertices.size()) - vVerts.size()) + iIndices[i];
				indices.push_back(indexNumber);

				indexNumber = (unsigned int)((loadedVertices.size()) - vVerts.size()) + iIndices[i];
				indices.push_back(indexNumber);
			}
		}

		//	Get mesh material name
		if (firstToken(currentLine) == "usemtl")
		{
			meshMaterialNames.push_back(tail(currentLine));

			//	Create new mesh if material changes within group
			if (!indices.empty() && !vertices.empty())
			{
				tempMesh = Mesh(vertices, indices);
				tempMesh.name = meshName;
				int i = 2;
				while (true) {
					tempMesh.name = meshName + "_" + std::to_string(i);

					for (auto& m : loadedMeshes)
					{
						if (m.name == tempMesh.name)
						{
							continue;
						}
					}
					break;
				}

				//	insert mesh
				loadedMeshes.push_back(tempMesh);
			}

			#ifdef LOADER_CONSOLE_OUTPUT
			outputIndicator = 0;
			#endif
		}

		// Load materials
		if (firstToken(currentLine) == "mtlib")
		{
			std::vector<std::string> temp;
			split(path, temp, "/");

			std::string pathToMaterial = "";

			if (temp.size() != 1)
			{
				for (int i = 0; i < temp.size() - 1; i++)
				{
					pathToMaterial += temp[i] + "/";
				}
			}

			pathToMaterial += tail(currentLine);

			#ifdef LOADER_CONSOLE_OUTPUT
			std::cout << std::endl << "- find materials in: " << pathToMaterial << std::endl;
			#endif

			loadMaterials(pathToMaterial);
		}
	}
	#ifdef LOADER_CONSOLE_OUTPUT
	std::cout << std::endl;
	#endif

	//	Deal with last mesh
	if (!indices.empty() && !vertices.empty())
	{
		//	Create mesh
		tempMesh = Mesh(vertices, indices);
		tempMesh.name = meshName;

		//	Insert mesh
		loadedMeshes.push_back(tempMesh);
	}

	file.close();

	//	Set materials for each mesh
	for (int i = 0; i < meshMaterialNames.size(); i++)
	{
		std::string materialName = meshMaterialNames[i];

		//find material in loaded materials and load into mesh
		for (int j = 0; j < loadedMaterials.size(); j++)
		{
			if (loadedMaterials[j].name == materialName)
			{
				loadedMeshes[i].meshMaterial = loadedMaterials[j];
				break;
			}
		}
	}

	if (loadedMeshes.empty() && loadedVertices.empty() && loadedIndices.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

//	Generate vertices from a list of positions, tcords, normals and face line
void Loader::generateVertices(std::vector<Vertex>& oVerts, const std::vector<Vector3f>& iPositions,
	const std::vector<Vector2f>& iTCoords, const std::vector<Vector3f>& iNormals, std::string iCurrentLine)
{
	std::vector<std::string> sFace, sVert;
	Vertex vVert;
	split(tail(iCurrentLine), sFace, " ");

	bool noNormal = false;

	for (int i = 0; i<int(sFace.size()); i++)
	{
		int vertexType;
		split(sFace[i], sVert, "/");

		//	Check for position - v1 or position and texture- v1/vt1
		if (sVert.size() == 1 || sVert.size() == 2)
		{
			vertexType = sVert.size();
		}
		//	Check for position, texture and normal - v1/vt1/vn1
		//	or position and normal - v1/vn1
		if (sVert.size() == 3)
		{
			if (sVert[1] != "")
			{
				vertexType = 4;		//	position, texture, normal
			}
			else
			{
				vertexType = 3;		//	position, normal
			}
		}

		//	Calculate and store the vertex
		switch (vertexType)
		{
		case 1:
		{
			vVert.position = getElement(iPositions, sVert[0]);
			vVert.textureCoordinate = Vec2f(0, 0);
			noNormal = true;
			oVerts.push_back(vVert);
			break;
		}
		case 2:
		{
			vVert.position = getElement(iPositions, sVert[0]);
			vVert.textureCoordinate = getElement(iTCoords, sVert[1]);
			noNormal = true;
			oVerts.push_back(vVert);
			break;
		}
		case 3:
		{
			vVert.position = getElement(iPositions, sVert[0]);
			vVert.textureCoordinate = Vec2f(0, 0);
			vVert.normal = getElement(iNormals, sVert[2]);
			oVerts.push_back(vVert);
			break;
		}
		case 4:
		{
			vVert.position = getElement(iPositions, sVert[0]);
			vVert.textureCoordinate = getElement(iTCoords, sVert[1]);
			vVert.normal = getElement(iNormals, sVert[2]);
			oVerts.push_back(vVert);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	// take care of missing normals
	if (noNormal)
	{
		Vec3f A = oVerts[0].position - oVerts[1].position;
		Vec3f B = oVerts[2].position - oVerts[1].position;

		Vec3f normal = cross(A, B);

		for (int i = 0; i< int(oVerts.size()); i++)
		{
			oVerts[i].normal = normal;
		}
	}
}

//	Triangulate a list of vertices into a face
void Loader::vertexTriangulation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts)
{
	//	if less than 3 verts - can'r create triangle - exit
	if (iVerts.size() < 3)
	{
		return;
	}
	//	if it's a triangle - finish
	if (iVerts.size() == 3)
	{
		oIndices.push_back(0);
		oIndices.push_back(1);
		oIndices.push_back(2);
		return;
	}

	//	if it's not a triangle, and triangle can be created

	std::vector<Vertex> tVerts = iVerts;

	while (true)
	{
		for (int i = 0; i<int(tVerts.size()); i++)
		{
			Vertex previousVertex;
			if (i == 0)
			{
				previousVertex = tVerts[tVerts.size() - 1];
			}
			else
			{
				previousVertex = tVerts[i - 1];
			}

			Vertex currentVertex = tVerts[i];
			Vertex nextVertex;
			if (i == tVerts.size() - 1)
			{
				nextVertex = tVerts[0];
			}
			else
			{
				nextVertex = tVerts[i + 1];
			}

			//	If three vertices left - it's the last triangle
			if (tVerts.size() == 3)
			{
				for (int j = 0; j<int(tVerts.size()); j++)
				{
					if (iVerts[j].position == currentVertex.position)
						oIndices.push_back(j);
					if (iVerts[j].position == previousVertex.position)
						oIndices.push_back(j);
					if (iVerts[j].position == nextVertex.position)
						oIndices.push_back(j);
				}

				tVerts.clear();
				break;
			}

			if (tVerts.size() == 4)
			{
				for (int j = 0; j<int(tVerts.size()); j++)
				{
					if (iVerts[j].position == currentVertex.position)
						oIndices.push_back(j);
					if (iVerts[j].position == previousVertex.position)
						oIndices.push_back(j);
					if (iVerts[j].position == nextVertex.position)
						oIndices.push_back(j);
				}

				Vec3f tempVector;
				for (int j = 0; j< int(tVerts.size()); j++)
				{
					if (tVerts[j].position != currentVertex.position
						&& tVerts[j].position != previousVertex.position
						&& tVerts[j].position != nextVertex.position)
					{
						tempVector = tVerts[j].position;
						break;
					}
				}

				for (int j = 0; j<int(tVerts.size()); j++)
				{
					if (iVerts[j].position == previousVertex.position)
						oIndices.push_back(j);
					if (iVerts[j].position == nextVertex.position)
						oIndices.push_back(j);
					if (iVerts[j].position == tempVector)
						oIndices.push_back(j);
				}

				tVerts.clear();
				break;
			}

			//	If vertex is not an interior vertex
			float angle = angleBetween(previousVertex.position - currentVertex.position, nextVertex.position - currentVertex.position);
			if (angle <= 0 && angle >= 180)
				continue;

			//	Check for vertices inside this triangle
			bool inside = false;
			for (int j = 0; j<int(iVerts.size()); j++)
			{
				if (inTriangle(iVerts[j].position, previousVertex.position, currentVertex.position, nextVertex.position)
					&& iVerts[j].position != previousVertex.position
					&& iVerts[j].position != currentVertex.position
					&& iVerts[j].position != nextVertex.position)
				{
					inside = true;
					break;
				}
			}
			if (inside)
			{
				continue;
			}

			for (int j = 0; j<int(tVerts.size()); j++)
			{
				if (iVerts[j].position == currentVertex.position)
					oIndices.push_back(j);
				if (iVerts[j].position == previousVertex.position)
					oIndices.push_back(j);
				if (iVerts[j].position == nextVertex.position)
					oIndices.push_back(j);
			}

			//	delete current vertex from the list
			for (int j = 0; j<int(tVerts.size()); j++)
			{
				if (tVerts[j].position == currentVertex.position)
				{
					tVerts.erase(tVerts.begin() + j);
					break;
				}
			}

			// reset loop
			i = -1;
		}

		//	 If no triagles were created
		if (oIndices.size() == 0)
		{
			break;
		}
		//	If no vertices left
		if (tVerts.size() == 0)
		{
			break;
		}
	}
}

//	load from .mtl file
bool Loader::loadMaterials(std::string path)
{
	return false;
}
