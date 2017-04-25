#include "node_button.h"
#include "print.h"


NodeButton::NodeButton(int x, int y, int r) : Button(x,y)
{
	this->x = x;
	this->y = y;
	this->r = r;
}

void NodeButton::print(HDC hdc)
{
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;

	EllipseLine(hdc, x,y,r,1,WHITE, BLACK);

	hPen = CreatePen(PS_SOLID, 2, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(BLACK);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	MoveToEx(hdc,x,y-10,NULL); 
	LineTo(hdc,x,y+10); 
	MoveToEx(hdc,x-10,y,NULL); 
	LineTo(hdc,x+10,y); 

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

/*void NodeButton::print(HDC hdc){ Node::print(hdc); }
bool NodeButton::isIn(int x, int y){ return Node::isIn(x, y); }
void NodeButton::LDown(){ Node::LDown(); }
bool NodeButton::LUp(){ return Node::LUp(); }*/