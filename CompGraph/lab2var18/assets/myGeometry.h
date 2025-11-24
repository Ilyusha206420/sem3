#pragma once
#include "matrix.h"

struct point {
    Matrix mat;
    point(float x, float y);
    float getX() {return mat[0][0];};
    float getY() {return mat[1][0];};
};

class Quadrilateral {
private:
    point _inst, A, B, C, D;
public:
    Quadrilateral(
        float instX, float instY,
        float x1, float y1,
        float x2, float y2,
        float x3, float y3,
        float x4, float y4);

    void move(float dx, float dy);
    void scale(float ds);
    void rotate(float dr);
    point getPoint(const char p);
};