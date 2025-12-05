#include "myWindow.h"
#include "Geometry.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <vector>

myWindow::myWindow(int sizeX, int sizeY, const char* title) 
{
  SDL_Init(SDL_INIT_VIDEO);
  this->_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizeX, sizeY, SDL_WINDOW_SHOWN);
  this->_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

myWindow::~myWindow() 
{
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
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

void myWindow::drawGeometry(Geometry& geom, int r, int g, int b, int a) 
{
  SDL_SetRenderDrawColor(_renderer, r, g, b, a);
  std::vector<point> points = geom.getPoints();
  std::vector<edge> edges = geom.getEdges();

  for (const auto &edge : edges) {
    SDL_RenderDrawLine(_renderer, 
      (int)((points[edge.a_ind].mat[0][0] - points[edge.a_ind].mat[2][0]) * 0.866), 
      (int)(points[edge.a_ind].mat[1][0] + ((points[edge.a_ind].mat[0][0] + points[edge.a_ind].mat[2][0]) / 2)), 
      (int)((points[edge.b_ind].mat[0][0] - points[edge.b_ind].mat[2][0]) * 0.866), 
      (int)(points[edge.b_ind].mat[1][0] + ((points[edge.b_ind].mat[0][0] + points[edge.b_ind].mat[2][0]) / 2)));
  }
}