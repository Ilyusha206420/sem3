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


bool isPointInsidePolygon(int x, int y, const std::vector<point>& points) {
    int n = points.size();
    bool inside = false;

    for (int i = 0, j = n - 1; i < n; j = i++) {
        int xi = points[i].mat[0][0], yi = points[i].mat[1][0];
        int xj = points[j].mat[0][0], yj = points[j].mat[1][0];

        if (((yi > y) != (yj > y)) &&
            (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
            inside = !inside;
            }
    }
    return inside;
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
            if(isPointInsidePolygon(x, y, points)) {
                SDL_RenderDrawPoint(_renderer, x, y);
            }
        }
    }
}
