#pragma once

#include "Event.h"
#include <vector>
#include <memory>
#include <optional>
#include <SDL2/SDL.h>

class WinInterface;

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
	explicit ControllerHandler(size_t _id, WinInterface& _win_interface);
    void handle_event(const SDL_Event& event);
    void update();

private:
    void handle_button_press(const SDL_Event& event);
    void handle_button_release(const SDL_Event& event);
    void handle_axis_motion(const SDL_Event& event);

	size_t id_;
    WinInterface& win_interface_;
    std::unique_ptr<SDL_GameController,ControllerDeleter> controller_;
    EventHandler event_handler_;
    bool active_ = false;
    bool main_ = false;
};