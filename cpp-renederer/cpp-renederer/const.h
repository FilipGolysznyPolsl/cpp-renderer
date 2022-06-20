#pragma once
#include"obraz.h"


const int WIDTH = 1000;
const int HEIGHT = 1000;
// Create image object
Obraz image(WIDTH, HEIGHT);
// Global model object
Model model;
// light direction vector
Vec3f lightDirection(0, 0, -1);
