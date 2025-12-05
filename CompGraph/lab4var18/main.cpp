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
    Geometry test({300, 0, 0}, 
      {{0, 0, 0}, 
      {80, 0, 0},
      {80, 0, 80},
      {0, 0, 80},
      {0, 80, 0}, 
      {80, 80, 0},
      {80, 80, 80},
      {0, 80, 80}});

    test.connect(0, 1);
    test.connect(1, 2);
    test.connect(2, 3);
    test.connect(0, 3);
    test.connect(0, 4);
    test.connect(1, 5);
    test.connect(2, 6);
    test.connect(3, 7);
    test.connect(4, 5);
    test.connect(5, 6);
    test.connect(6, 7);
    test.connect(7, 4);

    SDL_Event event;
    while(wind.handleEvents()) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        
        wind.clear();
        wind.drawGeometry(test, 0, 255, 0, 255);
        wind.present();
        wind.sleep(16);
    }
    return 0;
}
