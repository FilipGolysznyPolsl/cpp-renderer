#include <iostream>
#include<limits>
#include"vector2.h"
#include"vector3.h"
#include"obraz.h"


const int WIDTH = 400;
const int HEIGHT = 400;

Pixel red(255, 0 , 0);
Pixel white(255, 255, 255);
Pixel green(0, 255, 0);

Vec3f barycentric (Vec3f P, Vec3f* pts) {
    Vec3f u = cross(Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x), Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y));

    if (std::abs(u.z) > 1e-2) //    If u[2] is zero then triangle ABC is degenerate
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

bool inside(Vec3f P, Vec3f* pts) {
    Vec3f z = barycentric(P, pts);

    if (z.x < 0 || z.y < 0 || z.z < 0) {
        return false;
    }
    else {
        return true;
    }
}

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

int main()
{
	// Create image object
	Obraz image(WIDTH, HEIGHT);
       
    Vec3f a(200, 200, 200);
    Vec3f b(150, 150, 150);
    Vec3f c(15, 150, 30);

    Vec3f pts[3] = { a,b,c };
    
    triangle(pts, image, red);
    
    /*image.SetKolor(white, a.x, a.y);
    image.SetKolor(white, b.x, b.y);
    image.SetKolor(white, c.x, c.y);*/

	// Export created bmp file
	image.Export("../test1.bmp");
    return 0;
}