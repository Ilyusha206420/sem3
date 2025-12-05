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

struct edge
{
  size_t a_ind, b_ind;
  edge(size_t a, size_t b);
};

class Geometry 
{
  private:
    point _inst;
    std::vector<point> _points;
    std::vector<edge> _edges;
  public:
    Geometry(std::initializer_list<float> inst, std::initializer_list<std::initializer_list<float>> points);
    void connect(size_t a, size_t b);
    
    std::vector<point> getPoints();
    std::vector<edge> getEdges() { return std::vector<edge> (this->_edges); };

    void move(float dx, float dy, float dz);
    void rotateX(float dax);
    void rotateY(float day);
    void rotateZ(float daz);
    void scale(float ds);
};