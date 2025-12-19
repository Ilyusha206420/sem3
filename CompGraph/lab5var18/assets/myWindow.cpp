#include "myWindow.h"
#include "Geometry.h"
#include "matrix.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cstdint>
#include <vector>
#include <iostream>

myWindow::myWindow(int sizeX, int sizeY, const char* title) :
_xsize(sizeX),
_ysize(sizeY)
{
  SDL_Init(SDL_INIT_VIDEO);
  this->_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizeX, sizeY, SDL_WINDOW_SHOWN);
  this->_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  this->_zbuf = new int*[sizeY];
  for (int i = 0; i < sizeY; i++)
    this->_zbuf[i] = new int[sizeX];
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

  Matrix offset12 = {1, 4};
  Matrix offset23 = {1, 4};
  Matrix offset13 = {1, 4};
  Matrix &xtgt = offset12;
  int i = 0;
  for (const auto &face : faces) {
    SDL_SetRenderDrawColor(_renderer, face._color[0], face._color[1], face._color[2], face._color[3]);    
    offset12 = points[face._points[1]].mat + points[face._points[0]].mat * (-1);
    offset23 = points[face._points[2]].mat + points[face._points[1]].mat * (-1);
    offset13 = points[face._points[2]].mat + points[face._points[0]].mat * (-1);
    yiterations = points[face._points[0]].mat[1][0] > points[face._points[2]].mat[1][0] ? -offset13[1][0] : offset13[1][0];
    std::cout << yiterations << std::endl;
    i = 0;
    for (int y = points[face._points[0]].mat[1][0]; y != (int)points[face._points[2]].mat[1][0]; points[face._points[0]].mat[1][0] > points[face._points[2]].mat[1][0] ? y-- : y++) {
      // for (int x = points[face._points[0]].mat[0][0] + offset13[0][0])
      i++;
    }
    
    // SDL_RenderDrawLine(_renderer, 
    //   (int)((points[face._points[0]].mat[0][0] - points[face._points[0]].mat[2][0]) * 0.866), 
    //   (int)(-points[face._points[0]].mat[1][0] + ((points[face._points[0]].mat[0][0] + points[face._points[0]].mat[2][0]) / 2)), 
    //   (int)((points[face._points[1]].mat[0][0] - points[face._points[1]].mat[2][0]) * 0.866), 
    //   (int)(-points[face._points[1]].mat[1][0] + ((points[face._points[1]].mat[0][0] + points[face._points[1]].mat[2][0]) / 2)) 
    // );
    // SDL_RenderDrawLine(_renderer, 
    //   (int)((points[face._points[0]].mat[0][0] - points[face._points[0]].mat[2][0]) * 0.866), 
    //   (int)(-points[face._points[0]].mat[1][0] + ((points[face._points[0]].mat[0][0] + points[face._points[0]].mat[2][0]) / 2)), 
    //   (int)((points[face._points[2]].mat[0][0] - points[face._points[2]].mat[2][0]) * 0.866), 
    //   (int)(-points[face._points[2]].mat[1][0] + ((points[face._points[2]].mat[0][0] + points[face._points[2]].mat[2][0]) / 2)) 
    // );
    // SDL_RenderDrawLine(_renderer, 
    //   (int)((points[face._points[2]].mat[0][0] - points[face._points[2]].mat[2][0]) * 0.866), 
    //   (int)(-points[face._points[2]].mat[1][0] + ((points[face._points[2]].mat[0][0] + points[face._points[2]].mat[2][0]) / 2)), 
    //   (int)((points[face._points[1]].mat[0][0] - points[face._points[1]].mat[2][0]) * 0.866), 
    //   (int)(-points[face._points[1]].mat[1][0] + ((points[face._points[1]].mat[0][0] + points[face._points[1]].mat[2][0]) / 2)) 
    // );
  }
  // for (const auto &edge : edges) {
  //   SDL_RenderDrawLine(_renderer, 
  //     (int)((points[edge.a_ind].mat[0][0] - points[edge.a_ind].mat[2][0]) * 0.866), 
  //     (int)(-points[edge.a_ind].mat[1][0] + ((points[edge.a_ind].mat[0][0] + points[edge.a_ind].mat[2][0]) / 2)), 
  //     (int)((points[edge.b_ind].mat[0][0] - points[edge.b_ind].mat[2][0]) * 0.866), 
  //     (int)(-points[edge.b_ind].mat[1][0] + ((points[edge.b_ind].mat[0][0] + points[edge.b_ind].mat[2][0]) / 2)));
  // }
}