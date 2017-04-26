#include <math.h>
#include "plot_button.h"
#include "print.h"

#define PI 3.14159265358979

PlotButton::PlotButton(int x, int y, int r) : Button(x,y)
{
	plot_mode = false;
	this->x = x;
	this->y = y;
	this->r = r;
}

void PlotButton::print(HDC hdc)
{
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;

	if(plot_mode)
		EllipseLine(hdc, x,y,r,5,BRIGHTWHITE, BLACK);
	else
		EllipseLine(hdc, x,y,r,1,WHITE, BLACK);

	hPen = CreatePen(PS_SOLID, 1, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(WHITE);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	POINT tri[3];
	tri[0].x = (LONG)(x+10*cos(120*PI/180));
	tri[0].y = (LONG)(y+10*sin(120*PI/180));

	tri[1].x = (LONG)(x+10*cos(0*PI/180));
	tri[1].y = (LONG)(y+10*sin(0*PI/180));

	tri[2].x = (LONG)(x+10*cos(-120*PI/180));
	tri[2].y = (LONG)(y+10*sin(-120*PI/180));

	Polygon(hdc, tri, 3);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

/*void NodeButton::print(HDC hdc){ Node::print(hdc); }
bool NodeButton::isIn(int x, int y){ return Node::isIn(x, y); }
void NodeButton::LDown(){ Node::LDown(); }
bool NodeButton::LUp(){ return Node::LUp(); }*/