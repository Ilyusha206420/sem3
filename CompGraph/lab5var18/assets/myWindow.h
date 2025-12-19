#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstddef>
#include "Geometry.h"

class myWindow 
{
  private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    int **_zbuf;
    size_t _ysize;
    size_t _xsize;
  public:
    myWindow(int sizeX, int sizeY, const char* title);
    ~myWindow();

    void clear();
    void present() { SDL_RenderPresent(_renderer); };
    void sleep(int ms) { SDL_Delay(ms); };
    bool handleEvents();
    void drawGeometry(Geometry& geom);
};