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

	GetCursorPos(&current_position_);
	SetCursorPos(current_position_.x + x, current_position_.y + y);
}

#include <iostream>

void WinInterface::click_left_button()
{
	GetCursorPos(&current_position_);
	mouse_event(MOUSEEVENTF_LEFTDOWN, current_position_.x, current_position_.y, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, current_position_.x, current_position_.y, 0, 0);
}

void WinInterface::click_right_button()
{
	GetCursorPos(&current_position_);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, current_position_.x, current_position_.y, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, current_position_.x, current_position_.y, 0, 0);
}
