#include "assets/matrix.h"
#include "assets/myWindow.h"
#include "assets/Geometry.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

int main() {
    float cosDr = std::cos(0.05);
    float sinDr = std::sin(0.05);
    myWindow wind(600, 400, "Lab3Var18");
    Geometry quad({{300, 200}, 
                    {-80, -80},
                    {80, -80},
                    {80, 80},
                    {-80, 80},
                    {-120, 0}});
    SDL_Event event;
    while(wind.handleEvents()) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_UP])    quad.move(Matrix(1, 3, {{0}, {-5}, {0}}));
        if(keys[SDL_SCANCODE_DOWN])  quad.move(Matrix(1, 3, {{0}, {5}, {0}}));
        if(keys[SDL_SCANCODE_RIGHT]) quad.move(Matrix(1, 3, {{5}, {0}, {0}}));
        if(keys[SDL_SCANCODE_LEFT])  quad.move(Matrix(1, 3, {{-5}, {0}, {0}}));

        if(keys[SDL_SCANCODE_Z])     quad.transform(Matrix(3, 3, {
            {1.1, 0, 0},
            {0, 1.1, 0},
            {0, 0, 1}
        }));
        if(keys[SDL_SCANCODE_X])     quad.transform(Matrix(3, 3, {
            {0.9, 0, 0},
            {0, 0.9, 0},
            {0, 0, 1}
        }));

        if(keys[SDL_SCANCODE_R])     quad.transform(Matrix(3, 3, {
            {cosDr, -sinDr, 0},
            {sinDr,cosDr, 0},
            {0, 0, 1}
        }));
        if(keys[SDL_SCANCODE_E])     quad.transform(Matrix(3, 3, {
            {cosDr, sinDr, 0},
            {-sinDr,cosDr, 0},
            {0, 0, 1}
        }));

        wind.clear();
        wind.drawGeometry(quad, 0, 255, 0, 255);
        wind.fill(quad, 0, 255, 0, 255);
        wind.present();
        wind.sleep(16);
    }
    return 0;
}
