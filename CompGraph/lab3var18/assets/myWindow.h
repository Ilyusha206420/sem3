#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include "Geometry.h"
class myWindow {
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
public:
    myWindow(int sizeX, int sizeY, const char* title);
    ~myWindow();

    void clear();
    void present();
    void sleep(int ms);
    bool handleEvents();
    void drawGeometry(Geometry& geom, int r, int g, int b, int a);
    void fill(Geometry& geom, int r, int g, int b, int a);
};