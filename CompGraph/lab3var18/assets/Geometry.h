#pragma once
#include "matrix.h"
#include <initializer_list>
#include <vector>

struct point {
    Matrix mat;
    point(std::initializer_list<float> coords);
    point(Matrix a);
};

class Geometry {
private:
    std::vector<point> _points;
public:
    Geometry(std::initializer_list<std::initializer_list<float>> points);
    std::vector<point> getPoints();
    void move(const Matrix& m);
    void transform(Matrix m);
};