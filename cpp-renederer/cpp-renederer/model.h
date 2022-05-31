#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
#include<vector>


#include"vector3.h"

//	Model Vertex object that holds
//	Position, Normal, and Texture Coordinate
struct Vertex
{
	Vec3f position;
	Vec3f normal;
	Vec3f textureCoordinate;
};

//	Material struct holding variables for material and mappings
struct Material
{
	// Initialisation
	Material() 
	{
		name;
		specularExponent = 0.0f;
		opticalDensity = 0.0f;
		dissolve = 0.0f;
		ilumination = 0;
	}

	std::string name;

	Vec3f ambientColor;
	Vec3f diffuseColor;
	Vec3f specularColor;

	float specularExponent;
	float opticalDensity;
	float dissolve;
	int ilumination;

	std::string mapAmbientTexture;
	std::string mapDiffuseTexture;
	std::string mapSpecualrTexture;
	std::string mapSpecularHighlights;
	std::string mapAlphaTexture;
	std::string mapBump;

};

//	Mesh Object that holds
//	name, vertex list, and an index list
struct Mesh 
{
	Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices) {
		vertices = _Vertices;
		indices = _Indices;
	}
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material meshMaterial;
};