#include "WinInterface.h"

WinInterface::WinInterface()
{
	GetCursorPos(&current_position_);
}

void WinInterface::move_cursor(int _x, int _y)
{
	SetCursorPos(current_position_.x + _x, current_position_.y + _y);
	GetCursorPos(&current_position_);
}
