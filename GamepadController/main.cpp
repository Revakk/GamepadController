#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include "SystemControl.h"

int main(int argc, char* args[])
{
    SDL_SetMainReady();
    // Initialize SDL video and game controller subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
        return -1;
    }

    SystemController s_c;

    while (1)
    {
        s_c.update();
    }
    
    SDL_Quit();
    return 0;
}
