#include "assets/myWindow.h"
#include "assets/Geometry.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

int main() 
{
  float cosDr = std::cos(0.05);
  float sinDr = std::sin(0.05);
  myWindow wind(600, 400, "Lab3Var18");
  Geometry geometry({300, 0, 0}, 
    {{0, 0, 0}, 
    {80, 0, 0},
    {80, 0, 80},
    {0, 0, 80},
    {0, 80, 0}, 
    {80, 80, 0},
    {80, 80, 80},
    {0, 80, 80}});

  geometry.connect(0, 1);
  geometry.connect(1, 2);
  geometry.connect(2, 3);
  geometry.connect(0, 3);
  geometry.connect(0, 4);
  geometry.connect(1, 5);
  geometry.connect(2, 6);
  geometry.connect(3, 7);
  geometry.connect(4, 5);
  geometry.connect(5, 6);
  geometry.connect(6, 7);
  geometry.connect(7, 4);

  SDL_Event event;
  while (wind.handleEvents()) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RIGHT])  geometry.move(5, 0, 0);
    if (keys[SDL_SCANCODE_LEFT])   geometry.move(-5, 0, 0);
    if (keys[SDL_SCANCODE_UP])     geometry.move(0, 5, 0);
    if (keys[SDL_SCANCODE_DOWN])   geometry.move(0, -5, 0);
    if (keys[SDL_SCANCODE_RALT])   geometry.move(0, 0, 5);
    if (keys[SDL_SCANCODE_RSHIFT]) geometry.move(0, 0, -5);

    


    wind.clear();
    wind.drawGeometry(geometry, 0, 255, 0, 255);
    wind.present();
    wind.sleep(16);
  }
  return 0;
}
