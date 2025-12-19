#pragma once
#include "matrix.h"
#include <cstddef>
#include <initializer_list>
#include <vector>

struct point 
{
  Matrix mat;
  point(std::initializer_list<float> coords);
  point(Matrix a);
};

struct face
{
  size_t _points[3];
  unsigned char _color[4];
  face(size_t a_ind, size_t b_ind, size_t c_ind, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

class Geometry 
{
  private:
    point _inst;
    std::vector<point> _points;
    std::vector<face> _faces;
  public:
    Geometry(std::initializer_list<float> inst, std::initializer_list<std::initializer_list<float>> points);
    void createFace(std::initializer_list<size_t> points, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddPoint(std::initializer_list<float> point);

    std::vector<point> getPoints();
    std::vector<face> getFaces() { return std::vector<face> (this->_faces); };

    void move(float dx, float dy, float dz);
    void rotateX(float dax);
    void rotateY(float day);
    void rotateZ(float daz);
    void scale(float ds);
};