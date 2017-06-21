#include "input_data.h"
#include "print.h"

InputData::InputData(long double x, long double y, int class_id)
{
	this->pos.x = x;
	this->pos.y = y;
	this->r = 3;
	this->class_id = class_id;
	if(class_id==0)
		color = WHITE;
	else if(class_id==1)
		color = RED;
}

int InputData::get_class()
{
	return class_id;
}

Coord<long double> InputData::getXY()
{
	return pos;
}
void InputData::print(HDC hdc, int zoom)
{
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;

	long double xpos = (pos.x + zoom/2.0)*240.0/zoom + 20;
	long double ypos = 250 - (pos.y + zoom/2.0)*240.0/zoom ;

	if(20 <= xpos && xpos <= 260 && 10 <= ypos && ypos <= 250)
		EllipseLine(hdc, xpos, ypos, r, 1, color, color);
}
