#include "myWindow.h"
#include "myGeometry.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

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

void myWindow::drawQuad(Quadrilateral& q, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawLine(_renderer, 
                    q.getPoint('A').getX(), q.getPoint('A').getY(), 
                    q.getPoint('B').getX(), q.getPoint('B').getY());
    SDL_RenderDrawLine(_renderer, 
                    q.getPoint('B').getX(), q.getPoint('B').getY(), 
                    q.getPoint('C').getX(), q.getPoint('C').getY());
    SDL_RenderDrawLine(_renderer, 
                    q.getPoint('C').getX(), q.getPoint('C').getY(), 
                    q.getPoint('D').getX(), q.getPoint('D').getY());
    SDL_RenderDrawLine(_renderer, 
                    q.getPoint('D').getX(), q.getPoint('D').getY(), 
                    q.getPoint('A').getX(), q.getPoint('A').getY());
}