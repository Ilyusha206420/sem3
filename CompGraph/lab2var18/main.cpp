#include "assets/myWindow.h"
#include "assets/myGeometry.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

int main() {
    myWindow wind(600, 400, "Lab2Var18");
    Quadrilateral quad(300, 200, 
                        50, 30,
                        80, -80,
                        80, 80,
                        -80, 80);
    while(wind.handleEvents()) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_UP])    quad.move(0, -5);
        if(keys[SDL_SCANCODE_DOWN])  quad.move(0, 5);
        if(keys[SDL_SCANCODE_RIGHT]) quad.move(5, 0);
        if(keys[SDL_SCANCODE_LEFT])  quad.move(-5, 0);

        if(keys[SDL_SCANCODE_Z])     quad.scale(1.1);
        if(keys[SDL_SCANCODE_X])     quad.scale(0.9);

        if(keys[SDL_SCANCODE_E])     quad.rotate(-0.05);
        if(keys[SDL_SCANCODE_R])     quad.rotate(0.05);

        wind.clear();
        wind.drawQuad(quad, 0, 255, 0, 255);
        wind.present();
        wind.sleep(16);
    }
    return 0;
}