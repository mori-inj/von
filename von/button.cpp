#include "button.h"
#include "print.h"

Button::Button(int x, int y)
{
	r = 25;
	this->pos.x = x;
	this->pos.y = y;
}

void Button::setXY(int x, int y)
{
	this->pos.x = x;
	this->pos.y = y;
}

Coord<int> Button::getXY()
{
	return pos;
}

int Button::getR()
{
	return r;
}

bool Button::isIn(int x, int y)
{
	if( (x - pos.x)*(x - pos.x) + (y - pos.y)*(y - pos.y) <= r*r)
	{
		return true;
	}
	return false;
}

void Button::LDown()
{
	lclicked = true;
}
bool Button::LUp()
{
	if(lclicked)
	{
		lclicked = false;
		return true;
	}
	return false;
}

void Button::RDown()
{
	rclicked = true;
}

bool Button::RUp()
{
	if(rclicked)
	{
		rclicked = false;
		return true;
	}
	return false;
}

void Button::print(HDC hdc)
{
	EllipseLine(hdc, pos.x, pos.y, r, 1, WHITE, BLACK);
}