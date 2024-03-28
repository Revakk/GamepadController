#pragma once
#include "Event.h"
#include "ControllerHandler.h"
#include "WinInterface.h"

class SystemController
{
public:
	SystemController() = default;
	SystemController(const SystemController&) = delete;
	SystemController(SystemController&&) = delete;
	SystemController operator=(SystemController) = delete;
	SystemController operator=(SystemController&&) = delete;

	void update();


private:
	std::map<size_t,std::unique_ptr<ControllerHandler>> controllers_;
	WinInterface win_interface_;
};

