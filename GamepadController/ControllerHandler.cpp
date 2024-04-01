#include "ControllerHandler.h"
#include <iostream>
#include "WinInterface.h"
#include <algorithm>
#include <ranges>


ControllerHandler::ControllerHandler(size_t _id, WinInterface& _win_interface) : id_(_id), win_interface_(_win_interface)
{
    if (SDL_IsGameController(_id))
    {
        controller_ = std::unique_ptr<SDL_GameController, ControllerDeleter>(SDL_GameControllerOpen(_id));
    }

    event_handler_.register_event(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK, [this](float x, float y)
    {
        win_interface_.move_cursor(x, y);
    }
    );

    event_handler_.register_event(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A, [this]()
    {
        win_interface_.click_left_button();
    }
    );

    event_handler_.register_event(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B, [this]()
    {
                win_interface_.click_right_button();
    }
    );

    event_handler_.register_event(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK, [this]()
        {
            main_request_callback_(std::nullopt);
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
        buttons_state_[event.cbutton.button].trigger = true;
        break;
    case SDL_CONTROLLERBUTTONUP:
        buttons_state_[event.cbutton.button].trigger = false;
        buttons_state_[event.cbutton.button].pressed = true;
        break;
    default:
        break;
    }
}

void ControllerHandler::update_input()
{   
    handle_axis_motion();
    handle_button_action();
}

void ControllerHandler::handle_button_action()
{
    for (auto &&[button, idx]: std::ranges::views::zip(buttons_state_, std::views::iota(0u)))
    {
        if (button.pressed)
        {
            event_handler_.trigger_event(static_cast<SDL_GameControllerButton>(idx));
            button.pressed = false;
        }
    }
}

void ControllerHandler::handle_axis_motion()
{
    if (!axis_in_deadzone())
    {
        auto [x_move, y_move] = get_axis_state();
        event_handler_.trigger_event(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK, x_move, y_move);
    }
}

bool ControllerHandler::axis_in_deadzone()
{
    auto [x_move,y_move] = get_axis_state();

    if (x_move < DEADZONE && x_move > -DEADZONE)
    {
        return true;
    }

    if (y_move < DEADZONE && y_move > -DEADZONE)
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
        return pattern_button < buttons_state_.size() && buttons_state_[pattern_button].trigger;
    }
    );

    if (pattern_pressed && button_combination_future_.has_value())
    {
        if (button_combination_future_.value().wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            std::wstring mb_string = L"Controller " + std::to_wstring(id_) + L" is now the main controller";

            std::thread([mb_string]()
            {
                MessageBox(NULL, mb_string.c_str(), L"Button combination pressed", MB_OK);
            }).detach(); 
            button_combination_future_ = std::nullopt;
            main_request_callback_(id_);
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

void ControllerHandler::set_request_callback(std::function<void(std::optional<size_t>)> _callback)
{
    this->main_request_callback_ = _callback;
}
