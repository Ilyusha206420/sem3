#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include "cross.h"

class Window {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    void drawBresenham(int x1, int y1, int x2, int y2, int r, int g, int b) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        
        while (true) {
            SDL_RenderDrawPoint(renderer, x1, y1);
            if (x1 == x2 && y1 == y2) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
    
public:
    Window(int w, int h, const char* title) {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
    
    ~Window() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    void drawBresenhamLine(const Line& line) {
        drawBresenham(line.start.x, line.start.y, line.end.x, line.end.y, line.r, line.g, line.b);
    }
    
    void drawSDLLine(const Line& line) {
        SDL_SetRenderDrawColor(renderer, line.r, line.g, line.b, 255);
        SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
    }
    
    void clear() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    
    void present() {
        SDL_RenderPresent(renderer);
    }
    
    bool handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) return false;
        }
        return true;
    }
};