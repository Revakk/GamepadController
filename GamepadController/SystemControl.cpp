#include "SystemControl.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>


/* TODO:
*  - test the program when you have main controller and it disconnects
*  - maybe more functionality? create game folder and open it with specific key
*  - open steam with specific key
*  - 
*/
void SystemController::update()
{
    auto event = SDL_Event();

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
        {
            controllers_.at(event.cbutton.which)->handle_event(event);
        }

        if (event.type == SDL_CONTROLLERDEVICEREMOVED)
        {
            auto device_id = event.cdevice.which;
            controllers_.erase(device_id);
            if (main_controller_id_.value_or((std::numeric_limits<size_t>::max)()) == device_id)
            {
                main_controller_id_.reset();
            }
        }

        if (event.type == SDL_CONTROLLERDEVICEADDED)
        {
            auto device_id = event.cdevice.which;
            controllers_.emplace(std::make_pair<size_t, std::unique_ptr<ControllerHandler>>(static_cast<size_t>(device_id), std::make_unique<ControllerHandler>(device_id, win_interface_)));
            controllers_.at(device_id)->set_request_callback(std::bind(&SystemController::swap_main_controller, this, std::placeholders::_1));
        }
    }

    if (main_controller_id_.has_value())
    {
        controllers_.at(main_controller_id_.value())->update_input();
    }
    else {
        std::for_each(controllers_.begin(), controllers_.end(), [](auto& controller) {controller.second->process_button_combinations(); });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}
