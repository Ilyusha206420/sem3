#include "assets/myWindow.h"
#include "assets/Geometry.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

int main() 
{
  myWindow wind(600, 400, "Lab4Var18");
  Geometry geometry({300, 0, 0}, 
    {{-40, -40, -40}, 
    {40, -40, -40},
    {40, -40, 40},
    {-40, -40, 40},
    {-40, 40, -40}, 
    {40, 40, -40},
    {40, 40, 40},
    {-40, 40, 40}});
  
  for (int i = 0; i < 3; i++) {
    geometry.connect(i, i + 1);
    geometry.connect(i, i + 4);
  }
  for (int i = 4; i < 7; i++)
    geometry.connect(i, i + 1);

  geometry.connect(0, 3);
  geometry.connect(3, 7);
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

    if (keys[SDL_SCANCODE_Q]) geometry.rotateX(0.05);
    if (keys[SDL_SCANCODE_A]) geometry.rotateX(-0.05);
    if (keys[SDL_SCANCODE_W]) geometry.rotateY(0.05);
    if (keys[SDL_SCANCODE_S]) geometry.rotateY(-0.05);
    if (keys[SDL_SCANCODE_E]) geometry.rotateZ(0.05);
    if (keys[SDL_SCANCODE_D]) geometry.rotateZ(-0.05);

    if (keys[SDL_SCANCODE_Z]) geometry.scale(0.9);
    if (keys[SDL_SCANCODE_X]) geometry.scale(1.1);

    wind.clear();
    wind.drawGeometry(geometry, 0, 255, 0, 255);
    wind.present();
    wind.sleep(16);
  }
  
  return 0;
}
