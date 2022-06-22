#pragma once

#include"vector3.h"
#include<vector>


//	Test if point 1 is on same sade of line segment ab as point 2
template<typename T>
bool sameSide(Vector3<T> p1, Vector3<T> p2, Vector3<T> a, Vector3<T> b)
{
	Vector3<T> cross1 = cross(b - a, p1 - a);
	Vector3<T> cross2 = cross(b - a, p2 - a);

	if (dot(cross1, cross2) >= 0) {
		return true;
	}
	else {
		return false;
	}
}

//	Calculate cross product normal for a triangle
template<typename T>
Vector3<T> triangleNormal(Vector3<T> a, Vector3<T> b, Vector3<T> c) 
{
	Vector3<T> u = b - a;
	Vector3<T> v = c - a;

	Vector3<T> normal = cross(u, v);

	return normal;
}

//	Check if 3D point p is within 3D triangle abc
template<typename T>
bool inTriangle(Vector3<T> p, Vector3<T> a, Vector3<T> b, Vector3<T> c)
{
	//	Check if it is inside the prism triangle outlines in 3D space
	bool inPrism = sameSide(p, a, b, c) && sameSide(p, b, a, c) && sameSide(p, c, a, b);

	//	If inPrism is false then it's outside prism - and outside triangle
	if (!inPrism)
		return false;

	//	Get triangle's normal, and project p onto nromal
	Vector3<T> normal = triangleNormal(a, b, c);
	Vector3<T> projectOntoNormal = projection(p, normal);

	//	P is on the triangle if distance from p to abc is 0
	if (magnitude(projectOntoNormal) == 0)
		return true;
	else
		return false;
}

// Split string into a string array at given token 
void split(std::string in, std::vector<std::string>& out, std::string token)
{
	out.clear();

	std::string temp;

	for (int i = 0; i < int(in.size()); i++)
	{
		std::string test = in.substr(i, token.size());

		if (test == token)
		{
			if (!temp.empty())
			{
				out.push_back(temp);
				temp.clear();
				i += (int)token.size() - 1;
			}
			else
			{
				out.push_back("");
			}
		}
		else if (i + token.size() >= in.size())
		{
			temp += in.substr(i, token.size());
			out.push_back(temp);
			break;
		}
		else
		{
			temp += in[i];
		}
	}
}

//	Get tail of string after the token
std::string tail(const std::string &in)
{
	size_t tokenStart = in.find_first_not_of(" /t");	//	Find token
	size_t spaceStart = in.find_first_of(" /t", tokenStart);	//	Find space after the start of token, to exclude it from next search
	size_t tailStart = in.find_first_not_of(" /t", spaceStart);	//	Find begining of tail
	size_t tailEnd = in.find_last_not_of(" /t");	//	Find end of tail

	if (tailStart != std::string::npos && tailEnd != std::string::npos)
	{
		return in.substr(tailStart, tailEnd - tailStart + 1);
	}
	else if (tailStart != std::string::npos)
	{
		return in.substr(tailStart);
	}
	return "";
}

//	Get first token of string
std::string firstToken(const std::string &in)
{
	if (!in.empty())
	{
		size_t tokenStart = in.find_first_not_of(" \t");
		size_t tokenEnd = in.find_first_of(" \t", tokenStart);
		
		if (tokenStart != std::string::npos && tokenEnd != std::string::npos)
		{
			return in.substr(tokenStart, tokenEnd - tokenStart);
		}
		else if (tokenStart != std::string::npos)
		{
			return in.substr(tokenStart);
		}
	}
	return "";
}

template<class T>
const T& getElement(const std::vector<T>& elements, std::string& index)
{
	int idx = std::stoi(index);
	if (idx < 0)
		idx = int(elements.size()) + idx;
	else
		idx--;
	return elements[idx];
}

