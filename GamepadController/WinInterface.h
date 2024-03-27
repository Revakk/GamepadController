#pragma once
#include <WinUser.h>
#include "windef.h"

class WinInterface
{
public:
	WinInterface();
	void move_cursor(int _x, int _y);

private:
	POINT current_position_;
};




