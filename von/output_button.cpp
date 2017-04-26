#include "output_button.h"
#include "print.h"


OutputButton::OutputButton(int x, int y, int r) : Button(x,y)
{
	this->x = x;
	this->y = y;
	this->r = r;
}

void OutputButton::print(HDC hdc)
{
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;

	EllipseLine(hdc, x,y,r,1,WHITE, BLACK);

	hPen = CreatePen(PS_SOLID, 2, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(BLACK);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	int size = 9;
	MoveToEx(hdc,x-size,y-size,NULL); 
	LineTo(hdc,x+size,y-size);
	LineTo(hdc,x+size,y+size);
	LineTo(hdc,x-size,y+size);
	LineTo(hdc,x-size,y-size);

	MoveToEx(hdc,x+18,y,NULL);
	LineTo(hdc,x+1,y);
	MoveToEx(hdc,x+18,y,NULL);
	LineTo(hdc,x+13,y-5);
	MoveToEx(hdc,x+18,y,NULL);
	LineTo(hdc,x+13,y+5);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}
