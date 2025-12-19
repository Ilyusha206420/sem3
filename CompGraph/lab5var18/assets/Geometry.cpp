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

face::face(size_t a_ind, size_t b_ind, size_t c_ind, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  this->_points[0] = a_ind;
  this->_points[1] = b_ind;
  this->_points[2] = c_ind;
  this->_color[0] = r;
  this->_color[1] = g;
  this->_color[2] = b;
  this->_color[3] = a;
}

Geometry::Geometry(std::initializer_list<float> inst, std::initializer_list<std::initializer_list<float>> points):
_inst(point(inst)) 
{
  for (const auto coords : points)
    this->_points.push_back(point(coords));
}

void Geometry::createFace(std::initializer_list<size_t> points, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  if (points.size() < 3) return;

  size_t p = points.begin()[0];
  for (int i = 1; i < points.size() - 1; i++) 
    this->_faces.push_back(face(p, points.begin()[i], points.begin()[i + 1], r, g, b, a));
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

void Geometry::rotateX(float dax)
{
  for (auto &point : this->_points)
    point.mat = Matrix(4, 4, {
  {1, 0, 0, 0},
  {0, std::cos(dax), std::sin(dax), 0},
  {0, -std::sin(dax), std::cos(dax), 0},
  {0, 0, 0, 1}}) * point.mat;
}

void Geometry::rotateY(float day)
{
  for (auto &point : this->_points)
    point.mat = Matrix(4, 4, {
  {std::cos(day), 0, std::sin(day), 0},
  {0, 1, 0, 0},
  {-std::sin(day), 0, std::cos(day), 0},
  {0, 0, 0, 1}}) * point.mat;
}

void Geometry::rotateZ(float daz)
{
  for (auto &point : this->_points)
    point.mat = Matrix(4, 4, {
  {std::cos(daz), -std::sin(daz), 0, 0},
  {std::sin(daz), std::cos(daz), 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}}) * point.mat;
}

void Geometry::scale(float ds)
{
  for (auto &point : this->_points)
    point.mat = Matrix(4, 4, {
  {ds, 0, 0, 0},
  {0, ds, 0, 0},
  {0, 0, ds, 0},
 {0, 0 ,0, 1}}) * point.mat;
}

void Geometry::AddPoint(std::initializer_list<float> p) {
  this->_points.push_back(point{p});
}