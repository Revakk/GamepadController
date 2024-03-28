#pragma once

#include "Event.h"
#include <vector>
#include <memory>
#include <optional>
#include <SDL2/SDL.h>

class WinInterface;

using AxisState = std::pair<int, int>;

class ControllerHandler
{
    struct ControllerDeleter {
        void operator()(SDL_GameController* ptr) const {
            if (ptr)
            {
                SDL_GameControllerClose(ptr);
            }
        }
    };

public:
	ControllerHandler(size_t _id, WinInterface& _win_interface);
    ControllerHandler(const ControllerHandler&) = delete;
    ControllerHandler& operator=(const ControllerHandler&) = delete;
    ControllerHandler(ControllerHandler&&) = delete;
    ControllerHandler& operator=(ControllerHandler&&) = delete;
    void handle_event(const SDL_Event& event);
    void update();

private:
    void handle_button_press(const SDL_Event& event);
    void handle_button_release(const SDL_Event& event);
    void handle_axis_motion();
    bool axis_in_deadzone();
    AxisState get_axis_state();

private:
    const int deadzone_ = 500;
	size_t id_;
    WinInterface& win_interface_;
    std::unique_ptr<SDL_GameController,ControllerDeleter> controller_;
    EventHandler event_handler_;
    bool active_ = false;
    bool main_ = false;
};