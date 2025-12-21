#include "myWindow.h"
#include "Geometry.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <vector>

myWindow::myWindow(int sizeX, int sizeY, const char* title) {
    SDL_Init(SDL_INIT_VIDEO);
    this->_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizeX, sizeY, SDL_WINDOW_SHOWN);
    this->_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

myWindow::~myWindow() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void myWindow::clear() {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
    }
    
void myWindow::present() {
    SDL_RenderPresent(_renderer);
}

void myWindow::sleep(int ms) {
    SDL_Delay(ms);
}

bool myWindow::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) return false;
    }
    return true;
}

void myWindow::drawGeometry(Geometry& geom, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    std::vector<point> points = geom.getPoints();
    point* last = &points[0];
    for(int i = 1; i < points.size(); i++) {
        SDL_RenderDrawLine(_renderer, 
            last->mat[0][0], last->mat[1][0], 
            points[i].mat[0][0], points[i].mat[1][0]);
        last = &points[i];
    }
    SDL_RenderDrawLine(_renderer, 
        points[0].mat[0][0], points[0].mat[1][0], 
        points[points.size() - 1].mat[0][0], points[points.size() - 1].mat[1][0]);
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

void myWindow::fill(Geometry& geom, int r, int g, int b, int a) {
    std::vector<point> points = geom.getPoints();
    if(points.size() < 3)
        return;

    int minX = points[0].mat[0][0], maxX = points[0].mat[0][0];
    int minY = points[0].mat[1][0], maxY = points[0].mat[1][0];

    for(int i = 1; i < points.size(); i++) {
        int x = points[i].mat[0][0];
        int y = points[i].mat[1][0];
        if(x < minX) minX = x;
        if(x > maxX) maxX = x;
        if(y < minY) minY = y;
        if(y > maxY) maxY = y;
    }

    SDL_SetRenderDrawColor(_renderer, r, g, b, a);

    for(int y = minY; y <= maxY; y++) {
        for(int x = minX; x <= maxX; x++) {
            if (inTriangle(x, y, 
            projections[0][0][0], projections[0][1][0],
            projections[1][0][0], projections[1][1][0],
            projections[2][0][0], projections[2][1][0])) 
                {
                    SDL_RenderDrawPoint(_renderer, x, y);
                }
        }
    }
}
