#pragma once
#include "Event.h"
#include "ControllerHandler.h"

class SystemControll
{
public:
	SystemControll();

private:
	std::vector<std::unique_ptr<ControllerHandler>> controllers_;
};

