#pragma once
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct Pixel {
	float r, g, b;

	Pixel();
	Pixel(float r, float g, float b);
	~Pixel();
};
class Obraz
{
public:
	Obraz(int w, int h);
	~Obraz();

	Pixel GetKolor(int x, int y) const;
	void SetKolor(const Pixel& kolor, int x, int y);

	void Export(const char* path);
	void Import(const char* path);

	int height;
	int width;
	vector<Pixel> v_pixel;
};

