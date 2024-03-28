#include "SystemControl.h"
#include <iostream>
#include <thread>
#include <chrono>

void SystemController::update()
{
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (!controllers_.contains(i))
        {
            controllers_.emplace(std::make_pair<size_t,std::unique_ptr<ControllerHandler>>(static_cast<size_t>(i),std::make_unique<ControllerHandler>(i,win_interface_)));
        }
    };

    auto event = SDL_Event();

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_CONTROLLERAXISMOTION || event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            //controllers_.at(event.cbutton.which)->handle_event(event);
        }
    }

    for (auto& controller : controllers_)
    {
        controller.second->update();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}
