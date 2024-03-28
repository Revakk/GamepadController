#include "ControllerHandler.h"
#include <iostream>
#include "WinInterface.h"

ControllerHandler::ControllerHandler(size_t _id, WinInterface& _win_interface) : id_(_id), win_interface_(_win_interface)
{
    if (SDL_IsGameController(_id))
    {
        controller_ = std::unique_ptr<SDL_GameController, ControllerDeleter>(SDL_GameControllerOpen(_id));
    }

    event_handler_.register_event(XboxControllerEvent::LStick, [this](float x, float y)
    {
        win_interface_.move_cursor(x, y);
    }
    );
}

void ControllerHandler::handle_event(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_CONTROLLERBUTTONDOWN:
        handle_button_press(event);
        break;
    case SDL_CONTROLLERBUTTONUP:
        handle_button_release(event);
        break;
    case SDL_CONTROLLERAXISMOTION:
        handle_axis_motion();
        break;
    default:
        break;
    }
}

void ControllerHandler::update()
{
    handle_axis_motion();
}

void ControllerHandler::handle_button_press(const SDL_Event& event)
{

}

void ControllerHandler::handle_button_release(const SDL_Event& event)
{
}

void ControllerHandler::handle_axis_motion()
{
    if (!axis_in_deadzone())
    {
        auto [x_move, y_move] = get_axis_state();
        event_handler_.trigger_event(XboxControllerEvent::LStick, x_move, y_move);
    }
}

bool ControllerHandler::axis_in_deadzone()
{
    auto [x_move,y_move] = get_axis_state();

    if (x_move < deadzone_ && x_move > -deadzone_)
    {
        return true;
    }

    if (y_move < deadzone_ && y_move > -deadzone_)
    {
        return true;
    }
    
    return false;
}

AxisState ControllerHandler::get_axis_state()
{
    auto x_move = SDL_GameControllerGetAxis(controller_.get(), SDL_CONTROLLER_AXIS_LEFTX);
    auto y_move = SDL_GameControllerGetAxis(controller_.get(), SDL_CONTROLLER_AXIS_LEFTY);
    return AxisState(x_move, y_move);
}
