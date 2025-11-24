#include "./assets/window.h"

int main() {
    Window window(800, 600, "Lab1");
    
    Cross cross(Point(300, 200), Point(500, 400),
                Point(400, 100), Point(400, 500));
    
    while (window.handleEvents()) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        
        if (keys[SDL_SCANCODE_LEFT]) cross.move(-3, 0);
        if (keys[SDL_SCANCODE_RIGHT]) cross.move(3, 0);
        if (keys[SDL_SCANCODE_UP]) cross.move(0, -3);  
        if (keys[SDL_SCANCODE_DOWN]) cross.move(0, 3);    
        
        if (keys[SDL_SCANCODE_Q]) cross.rotateLeft();
        if (keys[SDL_SCANCODE_E]) cross.rotateRight();
        
        if (keys[SDL_SCANCODE_Z]) cross.scaleUp();
        if (keys[SDL_SCANCODE_X]) cross.scaleDown();
        
        if (keys[SDL_SCANCODE_R]) cross.reset();
        
        window.clear();
        window.drawBresenhamLine(cross.getLine1());  // ! Красный - Брезенхэм
        window.drawSDLLine(cross.getLine2());        // * Зеленый - SDL
        window.present();
        
        SDL_Delay(16);
    }
    
    return 0;
}