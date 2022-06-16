#include <iostream>
#include<limits>
#include <map>
#include"vector2.h"
#include"vector3.h"
#include"obraz.h"
#include"loader.h"
#include"triangle.h"
#include"const.h"

Pixel loadColor(Vec2f P, Vec2f* pts, Vertex* verts) {
    Vec2f colorVert0 = verts[0].textureCoordinate;
    Vec3f w = barycentric(P, pts);
    return Pixel(0, 0, 0);
}

Vec2f convertCoords(Vec3f P) {
    Vec2f pScreen;
    Vec2f pNDC;
    Vec2f pRaster;

    pScreen.x = P.x / -P.z;
    pScreen.y = P.y / -P.z;
    //pNDC.x = (pScreen.x + model.canvasWidth * 0.5) / model.canvasWidth;
    pNDC.x = (pScreen.x + WIDTH * 0.5) / WIDTH;
    //pNDC.y = (pScreen.y + model.canvasHeight * 0.5) / model.canvasHeight;
    pNDC.y = (pScreen.y + HEIGHT * 0.5) / HEIGHT;
    pRaster.x = pNDC.x * WIDTH;
    pRaster.y = pNDC.y * HEIGHT;
    return pRaster;
}

int main(int argc, char** argv) {
    bool loaded = false;
    if (2 == argc) {
        loaded = model.loadFile(argv[1]);
    }
    else {
        //loaded = model.loadFile("obj/african_head.obj");
        //loaded = model.loadFile("obj/deer.obj");
        //loaded = model.loadFile("obj/box_stack.obj");
        loaded = model.loadFile("obj/man.obj");
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

        // Go through every 3rd index and print the
        //	triangle that these indices represent
        for (int j = 0; j < curMesh.indices.size(); j += 3)
        {
            //  Pick 3 indices corresponding to different vertices
            int i0 = curMesh.indices[j];
            int i1 = curMesh.indices[j + 1];
            int i2 = curMesh.indices[j + 2];
            /*std::cout << i0 << " | " << curMesh.vertices[i0].position << "     ";
            std::cout << i1 << " | " << curMesh.vertices[i1].position << "     ";
            std::cout << i2 << " | " << curMesh.vertices[i2].position << "\n";
            */
            //  Load the vertices for a triangle
            Vertex verts[3] = { curMesh.vertices[i0], curMesh.vertices[i1], curMesh.vertices[i2] };

            // Translate vert positions to screen coordinates
            Vec3f worldCoords[] = { verts[0].position, verts[1].position, verts[2].position };


            Vec2f screenCoords[3];
            for (int j = 0; j < 3; j++) {
                screenCoords[j] = convertCoords(worldCoords[j]);
                //screenCoords[j] = screenCoords[j] = Vec2f((worldCoords[j].x + 1.) * WIDTH / 2.f, (worldCoords[j].y + 1.) * HEIGHT / 2.f);
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