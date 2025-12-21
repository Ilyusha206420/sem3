#include "myWindow.h"
#include "Geometry.h"
#include "matrix.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <algorithm>
#include <cstdint>
#include <vector>

static const float cos45 = 0.785398;

myWindow::myWindow(int sizeX, int sizeY, const char* title) :
_xsize(sizeX),
_ysize(sizeY)
{
  SDL_Init(SDL_INIT_VIDEO);
  this->_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizeX, sizeY, SDL_WINDOW_SHOWN);
  this->_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  this->_zbuf = new double*[sizeY];
  for (int i = 0; i < sizeY; i++)
    this->_zbuf[i] = new double[sizeX];
}

myWindow::~myWindow() 
{
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
  for (int i = 0; i < _ysize; i++)
    delete this->_zbuf[i];
}

void myWindow::clear() 
{
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  SDL_RenderClear(_renderer);
}

bool myWindow::handleEvents() 
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) return false;
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) return false;
  }
  return true;
}

bool inTriangle(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3) 
{
  int v0x = x3 - x1;
  int v0y = y3 - y1;
  int v1x = x2 - x1;
  int v1y = y2 - y1;
  int v2x = x - x1;
  int v2y = y - y1;

  float dot00 = v0x * v0x + v0y * v0y;
  float dot01 = v0x * v1x + v0y * v1y;
  float dot02 = v0x * v2x + v0y * v2y;
  float dot11 = v1x * v1x + v1y * v1y;
  float dot12 = v1x * v2x + v1y * v2y;

  float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
  float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  return (u >= 0) && (v >= 0) && (u + v <= 1);
}

void getPlaneEquation(const Matrix& p1, const Matrix& p2, const Matrix& p3, 
                    double& A, double& B, double& C, double& D) 
{
  float v1x = p2[0][0] - p1[0][0], v1y = p2[1][0] - p1[1][0], v1z = p2[2][0] - p1[2][0];
  float v2x = p3[0][0] - p1[0][0], v2y = p3[1][0] - p1[1][0], v2z = p3[2][0] - p1[2][0];
  
  A = v1y * v2z - v1z * v2y;
  B = v1z * v2x - v1x * v2z;
  C = v1x * v2y - v1y * v2x;
  
  D = -(A * p1[0][0] + B * p1[1][0] + C * p1[2][0]);
}

void myWindow::drawGeometry(Geometry& geom) 
{
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  for (int i = 0; i < _ysize; i++) {
    for (int j = 0; j < _xsize; j++) {
      this->_zbuf[i][j] = INT32_MIN;
      SDL_RenderDrawPoint(_renderer, j, i);
    }
  }

  std::vector<point> points = geom.getPoints();
  std::vector<face> faces = geom.getFaces();
  int ymax = 0, xmax = 0;
  int ymin = 0, xmin = 0;
  double z = 0;
  Matrix projections[3] = {Matrix(1, 3), Matrix(1, 3), Matrix(1, 3)};
  double A = 0, B = 0, C = 0, D = 0;
  for (const auto &face : faces) {
    getPlaneEquation(points[face._points[0]].mat, points[face._points[1]].mat, points[face._points[2]].mat, A, B, C, D);
    for (int i = 0; i < 3; i++) {
      projections[i][0][0] = points[face._points[i]].mat[0][0];
      projections[i][1][0] = points[face._points[i]].mat[1][0];
    }
    
    ymax = std::max(std::max(projections[0][1][0], projections[1][1][0]), projections[2][1][0]);
    xmax = std::max(std::max(projections[0][0][0], projections[1][0][0]), projections[2][0][0]);
    ymin = std::min(std::min(projections[0][1][0], projections[1][1][0]), projections[2][1][0]);
    xmin = std::min(std::min(projections[0][0][0], projections[1][0][0]), projections[2][0][0]);
    
    ymax = ymax > _ysize ? _ysize : ymax;
    xmax = xmax > _xsize ? _xsize : xmax;
    SDL_SetRenderDrawColor(_renderer, face._color[0], face._color[1], face._color[2], face._color[3]);
    for (int y = ymin < 0 ? 0 : ymin; y < ymax; y++) {
      for (int x = xmin < 0 ? 0 : xmin; x < xmax; x++) {
        if (inTriangle(x, y, 
          projections[0][0][0], projections[0][1][0],
          projections[1][0][0], projections[1][1][0],
          projections[2][0][0], projections[2][1][0])) 
        {
          
          z = -((A * x) + (B * y) + D) / C;
          if(z > _zbuf[y][x]) {
            this->_zbuf[y][x] = z;
            SDL_RenderDrawPoint(_renderer, x, y);
          }
        }
      }
    }
  }
}