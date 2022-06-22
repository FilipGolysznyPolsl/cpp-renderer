#pragma once
#include"obraz.h"


const int WIDTH = 6000;
const int HEIGHT = 6000;
// Create image object
Obraz image(WIDTH, HEIGHT);
// Global model object
Model model;
// light direction vector
Vec3f lightDirection(0, 0, -1);
float zBuffer[WIDTH][HEIGHT]{};
