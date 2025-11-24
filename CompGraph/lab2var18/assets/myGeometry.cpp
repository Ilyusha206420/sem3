#include "myGeometry.h"
#include "matrix.h"
#include <cmath>

point::point(float x, float y): 
    mat(Matrix(1, 3, {
        {x},
        {y},
        {1}
    })) {}

Quadrilateral::Quadrilateral(
        float instX, float instY,
        float x1, float y1,
        float x2, float y2,
        float x3, float y3,
        float x4, float y4):
        _inst(point(instX, instY)),
        A(point(x1, y1)), B(point(x2, y2)), C(point(x3, y3)), D(point(x4, y4)) {}

void Quadrilateral::move(float dx, float dy) {
    this->_inst.mat += Matrix(1, 3, {
        {dx}, 
        {dy}, 
        {0}
    });
}

void Quadrilateral::scale(float ds) {
    Matrix sm(3, 3, {
        {ds, 0, 0},
        {0, ds, 0},
        {0, 0, 1}
    });
    this->A.mat *= sm;
    this->B.mat *= sm;
    this->C.mat *= sm;
    this->D.mat *= sm;
}

void Quadrilateral::rotate(float dr) {
    float cosDr = std::cos(dr);
    float sinDr = std::sin(dr);
    Matrix rm(3, 3, {
        {cosDr, -sinDr, 0},
        {sinDr, cosDr, 0},
        {0, 0, 1}
    });
    this->A.mat = rm * A.mat;
    this->B.mat = rm * B.mat;
    this->C.mat = rm * C.mat;
    this->D.mat = rm * D.mat;
}

point Quadrilateral::getPoint(const char p) {
    switch (p) {
        case 'A':
            return point(A.getX() + _inst.getX(), A.getY() + _inst.getY());
        case 'B':
            return point(B.getX() + _inst.getX(), B.getY() + _inst.getY());
        case 'C':
            return point(C.getX() + _inst.getX(), C.getY() + _inst.getY());
        case 'D':
            return point(D.getX() + _inst.getX(), D.getY() + _inst.getY());
        default:
            return point(0, 0);
    }
}