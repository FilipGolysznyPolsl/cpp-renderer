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

	std::vector<std::string> meshMatNames;

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
						<< (!meshMatNames.empty() ? "\t| material: " + meshMatNames.back() : "");
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
		}
	}
}

//	Generate vertices from a list of positions, tcords, normals and face line
void Loader::generateVertices(std::vector<Vertex>& oVerts, const std::vector<Vector3f>& iPositions,
	const std::vector<Vector2f>& iTCoords, const std::vector<Vector3f>& iNormals, std::string iCurline)
{

}

//	Triangulate a list of vertices into a face
void Loader::vertexTriangulation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts)
{
}

//	load from .mtl file
bool Loader::loadMaterials(std::string path)
{
	return false;
}
