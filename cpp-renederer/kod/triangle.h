#pragma once
#include"vector3.h"
#include"vector2.h"
#include"materials.h"
#include"obraz.h"
#include"const.h"

// Calculate edge function - calculate position of point P relative to vector ab
inline
float edgeFunction(const Vec2f& a, const Vec2f& b, const Vec2f& P)
{
    return (P.x - a.x) * (b.y - a.y) - (P.y - a.y) * (b.x - a.x);
}

//  Calculate barycentric coordinates for given point P in  3d triangle (pts[0],pts[1],pts[2])
Vec3f barycentric3D (Vec3f P, Vec3f* pts) {
    Vec3f u = cross(Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x), Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y));

    if (std::abs(u.z) > 1e-2) //    If u[2] is zero then triangle ABC is degenerate
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

//  Return barycentric coordinates of point P in 2d triangle of vertices in pts array
Vec3f barycentric(Vec2f P, Vec2f* pts) {
    float area = edgeFunction(pts[0], pts[2], pts[1]);
    Vec3f w;
    w.x = edgeFunction(pts[2], pts[1], P) / area;
    w.y = edgeFunction(pts[1], pts[0], P) / area;
    w.z = edgeFunction(pts[0], pts[2], P) / area;

    return w;
}

//  Sort vertices of triangle in clockwise order
void sortClockwise(Vec2f* &pts) {
    Vec2f top = pts[0];
    Vec2f middle = pts[1];
    Vec2f bottom = pts[2];

    if (bottom.y > middle.y) std::swap(middle, bottom);
    if (middle.y > top.y) std::swap(middle, top);
    if (bottom.y > middle.y) std::swap(bottom, middle);

    //  Check if middle point is on the right or left of bot-top vector
    if (edgeFunction(bottom, top, middle) > 0) {
        pts[0] = top;
        pts[1] = middle;
        pts[2] = bottom;
    }
    else
    {
        pts[0] = top;
        pts[1] = bottom;
        pts[2] = middle;
    }
}

//  Sort triangle vertices clockwise, for correct edge function computatioin
void sortClockwise(Vec3f &p1, Vec3f &p2, Vec3f &p3) {
    Vec2f pts[3];
    pts[0] = { p1.x, p1.y };
    pts[1] = { p2.x, p2.y };
    pts[2] = {p3.x, p3.y};

    Vec2f top = pts[0];
    Vec2f middle = pts[1];
    Vec2f bottom = pts[2];

    if (bottom.y > middle.y) std::swap(middle, bottom);
    if (middle.y > top.y) std::swap(middle, top);
    if (bottom.y > middle.y) std::swap(bottom, middle);


    //  Check if middle point is on the right or left of bot-top vector
    if (edgeFunction(bottom, top, middle) > 0) {
        p1.x = top.x;
        p1.y = top.y;
        p2.x = middle.x;
        p2.y = middle.y;
        p3.x = bottom.x;
        p3.y = bottom.y;
    }
    else
    {
        p1.x = top.x;
        p1.y = top.y;
        p2.x = bottom.x;
        p2.y = bottom.y;
        p3.x = middle.x;
        p3.y = middle.y;
    }
}

// Check if point P is inside triangle having vertices in pts array using barycentric coordinates
bool inside(Vec2f P, Vec2f* pts) {
    sortClockwise(pts);
    // assign weights from edge functions
    Vec3f w = barycentric(P, pts);

    if (w.x < 0 || w.y < 0 || w.z < 0) {
        return false;
    }
    else {
        return true;
    }
}

// Find z-coordinate of a point inside triangle 
float findZ(Vec2f P0, Vertex* verts) {
    Vec3f pts[3];
    Vec3f P;
    for (int i = 0; i < 3; i++) {
        pts[i].x = verts[i].position.x;
        pts[i].y = verts[i].position.y;
        pts[i].z = verts[i].position.z;
    }
    //sortClockwise(pts[0], pts[1], pts[2]);
    for (int i = 0; i < 3; i++) P.z += verts[i].position.z/3;
    
    Vec3f w = barycentric3D(P, pts);

    
    float z = w.x * verts[0].position.z + w.y * verts[1].position.z + w.z * verts[2].position.z;
    //float z = P.z;

    return z;
}


//  Draw triangle between points in pts array, on given image in given color
void triangle(Vec2f* pts, Obraz& image, Pixel& color, Vertex* verts) {
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
    Vec2f P;
    //Iterate over pixels inside the boundaries and run function to check id they're inside triangle
    for (P.x = boundaryMin.x; P.x < boundaryMax.x; P.x++) {
        for (P.y = boundaryMin.y; P.y < boundaryMax.y; P.y++) {
            if (inside(P, pts)) {

                float z= findZ(P, verts);

                if (zBuffer[int(P.x)][int(P.y)] < z) {
                    zBuffer[int(P.x)][int(P.y)] = z;
                    image.SetKolor(color, P.x, P.y);
                }
            }
        }
    }
}