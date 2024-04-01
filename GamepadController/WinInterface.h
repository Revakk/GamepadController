#pragma once
#include <windows.h>
#include <WinUser.h>

class WinInterface
{
public:
	WinInterface();
	void move_cursor(int _x, int _y);
	void click_left_button();
	void click_right_button();

private:
	POINT current_position_;
};