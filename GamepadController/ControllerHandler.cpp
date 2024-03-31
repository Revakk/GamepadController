#include "ControllerHandler.h"
#include <iostream>
#include "WinInterface.h"
#include <algorithm>


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

    buttons_state_.resize(SDL_CONTROLLER_BUTTON_MAX);
    buttons_state_ = std::vector<ButtonState>(SDL_CONTROLLER_BUTTON_MAX, ButtonState());
}

void ControllerHandler::handle_event(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_CONTROLLERBUTTONDOWN:
        handle_button_action(event,true);
        break;
    case SDL_CONTROLLERBUTTONUP:
        std::cout << "up" << '\n';
        handle_button_action(event,false);
        break;
    default:
        break;
    }
}

void ControllerHandler::update()
{
    handle_axis_motion();
    process_button_combinations();
}

void ControllerHandler::handle_button_action(const SDL_Event& event, bool trigger)
{
    if (event.type == SDL_CONTROLLERAXISMOTION)
    {
        if (event.cbutton.type == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
        {
            buttons_state_[event.cbutton.padding1].pressed = trigger;
        } 
        
        if (event.cbutton.type == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
        {
            buttons_state_[event.cbutton.padding2].pressed = trigger;
        }
    }
    else {
        buttons_state_[event.cbutton.button].pressed = trigger;
    }
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

void ControllerHandler::process_button_combinations()
{
    bool pattern_pressed = std::all_of(MAIN_REQUEST.begin(), MAIN_REQUEST.end(), [this](uint8_t pattern_button)
    {
        return pattern_button < buttons_state_.size() && buttons_state_[pattern_button].pressed;
    }
    );

    if (pattern_pressed && button_combination_future_.has_value())
    {
        if (button_combination_future_.value().wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            std::cout << "future is ready";
            //button_combination_future_.value().get();
        }
        else {
            std::cout << "future is not ready";
        }
    } else if (pattern_pressed && !button_combination_future_.has_value()){
        button_combination_future_ = std::async(std::launch::async, [this]()
        {
            start_request_timer();
        });
    }
    else {
        button_combination_future_ = std::nullopt;
    }
}

void ControllerHandler::start_request_timer()
{
    std::this_thread::sleep_for(std::chrono::seconds(REQUEST_TIMER));
}
