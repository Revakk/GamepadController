#include "ControllerHandler.h"
#include <iostream>

ControllerHandler::ControllerHandler(size_t _id, WinInterface& _win_interface) : id_(_id), win_interface_(_win_interface)
{
    if (SDL_IsGameController(_id))
    {
        controller_ = std::unique_ptr<SDL_GameController, ControllerDeleter>(SDL_GameControllerOpen(_id));
    }
}