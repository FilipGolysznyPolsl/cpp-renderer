#include <iostream>
#include<limits>
#include <map>
#include"vector2.h"
#include"vector3.h"
#include"obraz.h"
#include"loader.h"


const int WIDTH = 1000;
const int HEIGHT = 1000;
// Create image object
Obraz image(WIDTH, HEIGHT);

// Calculate edge function - calculate position of point P to vector ab
inline
float edgeFunction(const Vec3f &a, const Vec3f &b, const Vec3f &P)
{
    return (P.x - a.x) * (b.y - a.y) - (P.y - a.y) * (b.x - a.x);
}

////  Calculate barycentric coordinates for given point P in triangle (pts[0],pts[1],pts[2])
//Vec3f barycentric (Vec3f P, Vec3f* pts) {
//    Vec3f u = cross(Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x), Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y));
//
//    if (std::abs(u.z) > 1e-2) //    If u[2] is zero then triangle ABC is degenerate
//        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
//
//    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
//}

Vec3f barycentric(Vec3f P, Vec3f* pts) {
    float area = edgeFunction(pts[0], pts[2], pts[1]);
    Vec3f w;
    w.x = edgeFunction(pts[2], pts[1], P)/area;
    w.y = edgeFunction(pts[1], pts[0], P)/area;
    w.z = edgeFunction(pts[0], pts[2], P)/area;

    return w;
}

// Check if point P is inside triangle having vertices in pts array using barycentric coordinates
bool inside(Vec3f P, Vec3f* pts) {
    // assign weights from edge functions
    Vec3f w;
    w.x = edgeFunction(pts[2], pts[1], P);
    w.y = edgeFunction(pts[1], pts[0], P);
    w.z = edgeFunction(pts[0], pts[2], P);

    if (w.x < 0 || w.y < 0 || w.z < 0) {
        return false;
    }
    else {
        return true;
    }
}

//  Draw triangle between points in pts array, on given image in given color
void triangle(Vec3f* pts, Obraz& image, Pixel& color, Vertex* verts) {
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
            if (inside(P, pts)) {
                image.SetKolor(color, P.x, P.y);
            }
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
        //loaded = model.loadFile("obj/deer.obj");
        //loaded = model.loadFile("obj/box_stack.obj");
    }
    if (!loaded) return 1;

    Vec3f lightDirection(0, 0, -1);

    /*for (int h=0; h < model.loadedMeshes.size(); h++) {
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
    }*/

    // Go through each loaded mesh and out its contents
    for (int i = 0; i < model.loadedMeshes.size(); i++)
    {
        // Copy one of the loaded meshes to be our current mesh
        Mesh curMesh = model.loadedMeshes[i];
        std::cout << curMesh.name << "\n";

        // Go through every 3rd index and print the
        //	triangle that these indices represent
        for (int j = 0; j + 4 <= curMesh.indices.size(); j += 6)
        {
            //  Pick 3 indices corresponding to different vertices
            int i0 = curMesh.indices[j];
            int i1 = curMesh.indices[j + 2];
            int i2 = curMesh.indices[j + 4];
            /*std::cout << i0 << " | " << curMesh.vertices[i0].position << "     ";
            std::cout << i1 << " | " << curMesh.vertices[i1].position << "     ";
            std::cout << i2 << " | " << curMesh.vertices[i2].position << "\n";*/
            
            //  Load the vertices for a triangle
            Vertex verts[3] = { curMesh.vertices[i0], curMesh.vertices[i1], curMesh.vertices[i2] };

            // Translate vert positions to screen coordinates
            Vec3f worldCoords[] = { verts[0].position, verts[1].position, verts[2].position };
            Vec3f screenCoords[3];
            for (int j = 0; j < 3; j++) {
                screenCoords[j] = Vec3f((worldCoords[j].x + 1.) * WIDTH / 2.f, (worldCoords[j].y + 1.) * HEIGHT / 2.f, (worldCoords[j].z + 1.f));
            }

            //  Calculate light intensity
            Vec3f n = (cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]));
            normalize(n);
            float lightIntensity = dot(n, lightDirection);

            float l = lightIntensity * 255;

            Pixel grey(l, l, l);
            // Print triangle if it's lit
            if (lightIntensity > 0) {
                triangle(screenCoords, image, grey, verts);
            }

            //        srand(j);
            //        int index = rand() % 3;
            //        triangle(screenCoords, image, colors[index]);
        }
    }

	// Export created bmp file
	image.Export("../deer1.bmp");
    return 0;
}