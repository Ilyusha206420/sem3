#include "Geometry.h"
#include "matrix.h"
#include <initializer_list>
#include <vector>

point::point(std::initializer_list<float> coords):
    mat(Matrix(1, coords.size() + 1)) {
    int i = 0;
    for(const auto n : coords)
        this->mat[i++][0] = n;
    this->mat[i][0] = 1;
}

point::point(Matrix a): mat(a) {}

Geometry::Geometry(std::initializer_list<std::initializer_list<float>> points) {
    for(const auto coords : points)
        this->_points.push_back(point(coords));
}

std::vector<point> Geometry::getPoints() {
    std::vector<point> out;
    point inst = _points[0];
    for(int i = 1; i < _points.size(); i++) {
        out.push_back(point(inst.mat + _points[i].mat));
    }
    for(int i = 0; i < out.size(); i++)
        out[i].mat[out[i].mat.getY() - 1][0] -= 1;
    return out;
}

void Geometry::move(const Matrix& m) {
    for(auto& p : _points)
        p.mat += m;
}

void Geometry::transform(Matrix m) {
    for(int i = 1; i < _points.size(); i++)
        this->_points[i].mat = m * this->_points[i].mat;
}