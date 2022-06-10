﻿#include <iostream>
#include<limits>
#include <map>
#include"vector2.h"
#include"vector3.h"
#include"obraz.h"
#include"loader.h"


const int WIDTH = 800;
const int HEIGHT = 800;
// Create image object
Obraz image(WIDTH, HEIGHT);

std::map<std::string, Pixel> my_map = {
    
};
Pixel colors[] = {
    Pixel(255, 0 , 0),
    Pixel(255, 255, 255),
    Pixel(0, 255, 0),
    Pixel(200, 155, 255),
    Pixel(10, 255, 110),
    Pixel(0,0,255),
    Pixel(50, 255, 70),
    Pixel(200, 110, 255),
    Pixel(10, 230, 150),
};


//  Calculate barycentric coordinates for given point P in triangle (pts[0],pts[1],pts[2])
Vec3f barycentric (Vec3f P, Vec3f* pts) {
    Vec3f u = cross(Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x), Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y));

    if (std::abs(u.z) > 1e-2) //    If u[2] is zero then triangle ABC is degenerate
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}


// Check if point P is inside triangle having vertices in pts array using barycentric coordinates
bool inside(Vec3f P, Vec3f* pts) {
    Vec3f z = barycentric(P, pts);

    if (z.x < 0 || z.y < 0 || z.z < 0) {
        return false;
    }
    else {
        return true;
    }
}

//  Draw triangle between points in pts array, on given image in given color
void triangle(Vec3f* pts, Obraz& image, Pixel& color) {
    Vec2f boundaryMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f boundaryMax(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    Vec2f clamp(float(WIDTH - 1), float(HEIGHT - 1));
    //  Loop over provided points and find lowest and highest x,y values inside window borders
    //  to find the boundaries of the trangle
    for (int i = 0; i < 3; i++) {
        boundaryMin.x = std::max(0.f, std::min(boundaryMin.x, pts[i].x));
        boundaryMin.y = std::max(0.f, std::min(boundaryMin.y, pts[i].y));

        boundaryMax.x = std::min(clamp.x, std::max(boundaryMax.x, pts[i].x));
        boundaryMax.y = std::min(clamp.y, std::max(boundaryMax.y, pts[i].y));
    }
    Vec3f P;    //  temporary Point (Vec3f variable)
    //Iterate over pixels inside the boundaries and run function to check id they're inside triangle
    for (P.x = boundaryMin.x; P.x < boundaryMax.x; P.x++) {
        for (P.y = boundaryMin.y; P.y < boundaryMax.y; P.y++) {
            if (!inside(P, pts)) {
                continue;
            }
            image.SetKolor(color, P.x, P.y);
        }
    }
}

int main(int argc, char** argv) {
    Model model;
    bool loaded = false;
    if (2 == argc) {
        loaded = model.loadFile(argv[1]);
    }
    else {
        loaded = model.loadFile("obj/african_head.obj");
        //loaded = model.loadFile("obj/box_stack.obj");
    }
    if (!loaded) return 1;

    Vec3f lightDirection(0, 0, -1);

    for (int h=0; h < model.loadedMeshes.size(); h++) {
        Mesh currentMesh = model.loadedMeshes[h];
        for (int i = 2; i < currentMesh.vertices.size(); i += 3)
        {
            Vec3f worldCoords[3] = { model.loadedVertices[i].position, model.loadedVertices[i - 1].position, model.loadedVertices[i - 2].position };
            Vec3f screenCoords[3];
            for (int j = 0; j < 3; j++) {
                screenCoords[j] = Vec3f((worldCoords[j].x + 1.) * WIDTH / 2.f, (worldCoords[j].y + 1.) * HEIGHT / 2.f, (worldCoords[j].z + 1.f));
            }

            Vec3f n = (cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]));
            normalize(n);
            float lightIntensity = dot(n,lightDirection);
            float l = lightIntensity * 255;
            Pixel grey(l, l, l);
            if (lightIntensity > 0) {
                triangle(screenCoords, image, grey);
            }
        }
    }

    // Go through each loaded mesh and out its contents
    //for (int i = 0; i < model.loadedMeshes.size(); i++)
    //{
    //    // Copy one of the loaded meshes to be our current mesh
    //    Mesh curMesh = model.loadedMeshes[i];

    //    // Print Mesh Name
    //    std::cout << "Mesh " << i << ": " << curMesh.name << "\n";

    //    // Print Vertices
    //    std::cout << "Vertices:\n";

    //    // Go through each vertex and print its number,
    //    //  position, normal, and texture coordinate
    //    /*for (int j = 0; j < curMesh.vertices.size(); j++)
    //    {
    //        std::cout << "V" << j << ": " <<
    //            "P(" << curMesh.vertices[j].position.x << ", " << curMesh.vertices[j].position.y << ", " << curMesh.vertices[j].position.z << ") " <<
    //            "N(" << curMesh.vertices[j].normal.x << ", " << curMesh.vertices[j].normal.y << ", " << curMesh.vertices[j].normal.z << ") " <<
    //            "TC(" << curMesh.vertices[j].textureCoordinate.x << ", " << curMesh.vertices[j].textureCoordinate.y << ")\n";
    //    }*/

    //    // Print Indices
    //    std:cout << "Indices:\n";

    //    // Go through every 3rd index and print the
    //    //	triangle that these indices represent
    //    for (int j = 0; j+4 <= curMesh.indices.size(); j += 6)
    //    {
    //        //std::cout << "T" << j / 3 << ": " << curMesh.indices[j] << ", " << curMesh.indices[j + 1] << ", " << curMesh.indices[j + 2] << "\n"; 
    //        int i1 = curMesh.indices[j];
    //        int i2 = curMesh.indices[j+2];
    //        int i3 = curMesh.indices[j + 4];
    //        std::cout << i1 << " | " << curMesh.vertices[i1].position << "     ";
    //        std::cout << i2 << " | " << curMesh.vertices[i2].position << "     ";
    //        std::cout << i3 << " | " << curMesh.vertices[i3].position << "\n";
    //        Vec3f worldCoords[] = { curMesh.vertices[i1].position, curMesh.vertices[i2].position, curMesh.vertices[i3].position };
    //        
    //        Vec3f screenCoords[3];
    //        for (int j = 0; j < 3; j++) {
    //            screenCoords[j] = Vec3f((worldCoords[j].x + 1.) * WIDTH / 2.f, (worldCoords[j].y + 1.) * HEIGHT / 2.f, (worldCoords[j].z + 1.f));
    //        }

    //        srand(j);
    //        int index = rand() % 3;
    //        triangle(screenCoords, image, colors[index]);
    //    }

    //    // Print Material
    //    /*file << "Material: " << curMesh.MeshMaterial.name << "\n";
    //    file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
    //    file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
    //    file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
    //    file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
    //    file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
    //    file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
    //    file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
    //    file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
    //    file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
    //    file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
    //    file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
    //    file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";*/

    //    // Leave a space to separate from the next mesh
    //    std::cout << "\n";
    //}

	// Export created bmp file
	image.Export("../test3.bmp");
    return 0;
}