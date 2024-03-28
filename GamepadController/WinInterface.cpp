#include "WinInterface.h"

WinInterface::WinInterface()
{
	GetCursorPos(&current_position_);
}

void WinInterface::move_cursor(int _x, int _y)
{
	//scale down the movement
	int x = _x / 5000.0;
	int y = _y / 5000.0;

	SetCursorPos(current_position_.x + x, current_position_.y + y);
	GetCursorPos(&current_position_);
}
