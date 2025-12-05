#include "Geometry.h"
#include "matrix.h"
#include <cstddef>
#include <initializer_list>
#include <vector>
#include <cmath>

point::point(std::initializer_list<float> coords):
mat(Matrix(1, coords.size() + 1)) 
{
  int i = 0;
  for (const auto n : coords)
    this->mat[i++][0] = n;
  this->mat[i][0] = 1;
}

point::point(Matrix a): 
mat(a) {}

edge::edge(size_t a, size_t b):
a_ind(a), b_ind(b) {}

Geometry::Geometry(std::initializer_list<float> inst, std::initializer_list<std::initializer_list<float>> points):
_inst(point(inst)) 
{
  for (const auto coords : points)
    this->_points.push_back(point(coords));
}

void Geometry::connect(size_t a, size_t b)
{
  if (a > _points.size() || b > _points.size())
    return;
  this->_edges.push_back(edge(a, b));
}

std::vector<point> Geometry::getPoints() 
{
  std::vector<point> out;
  for (auto p : _points) 
    out.push_back(point(p.mat + _inst.mat));
  
  return out;
}

void Geometry::move(float dx, float dy, float dz) 
{
  this->_inst.mat += Matrix(1, 4, {
    {dx},
    {dy},
    {dz},
    {0}});
}

void Geometry::rotate(float dax, float day, float daz)
{
  for (auto &point : this->_points)
    point.mat = Matrix(4, 4, {
  {1, 0, 0, 0},
  {0, std::cos(dax), std::sin(dax), 0},
  {0, -std::sin(dax), std::cos(dax), 0},
  {0, 0, 0, 1}}) * point.mat;
}