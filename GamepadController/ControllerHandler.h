#pragma once

#include "Event.h"
#include <vector>
#include <memory>
#include <optional>
#include <SDL2/SDL.h>
#include <chrono>
#include <numeric>
#include <array>
#include <future>

class WinInterface;

using AxisState = std::pair<int, int>;

constexpr int REQUEST_TIMER = 3;
constexpr std::array<uint8_t, 4> MAIN_REQUEST = std::array<uint8_t, 4>{ SDL_CONTROLLER_BUTTON_LEFTSTICK ,SDL_CONTROLLER_BUTTON_RIGHTSTICK , SDL_CONTROLLER_BUTTON_LEFTSHOULDER , SDL_CONTROLLER_BUTTON_RIGHTSHOULDER };

struct ButtonState {
    bool pressed = false;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> time_pressed = std::nullopt;
};

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
    void handle_button_action(const SDL_Event& event, bool trigger);
    void handle_axis_motion();
    bool axis_in_deadzone();
    AxisState get_axis_state();
    void process_button_combinations();
    void start_request_timer();

private:
    const int deadzone_ = 500;
	size_t id_;
    WinInterface& win_interface_;
    std::unique_ptr<SDL_GameController,ControllerDeleter> controller_;
    std::vector<ButtonState> buttons_state_;
    EventHandler event_handler_;
    std::optional<std::future<void>> button_combination_future_;
};