#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include "ControllerHandler.h"

int main(int argc, char* args[])
{
    SDL_SetMainReady();
    // Initialize SDL video and game controller subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
        return -1;
    }

    ControllerHandler controllerHandler(0);


    while (1)
    {
        auto event = controllerHandler.poll_controller_event();
        if (event.has_value())
        {
            if (event.value().type == SDL_CONTROLLERBUTTONDOWN)
            {
                std::cout << "Button " << event.value().button.timestamp << " pressed\n";
            }
        }
    }
    



    SDL_Quit();
    return 0;
}
