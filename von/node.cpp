#include "node.h"
#include "weight.h"
#include "print.h"


Node::Node(int x, int y)
{
	r = 25;
	this->x = x;
	this->y = y;
}

void Node::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Node::getX()
{
	return x;
}

int Node::getY()
{
	return y;
}

int Node::getR()
{
	return r;
}
void Node::printWeight(HDC hdc)
{
	for(int i=0; i<weight_list.size(); i++)
		weight_list[i]->print(hdc);
}
void Node::print(HDC hdc)
{
	
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;

	EllipseLine(hdc, x,y,r,1,WHITE, BLACK);

	hPen = CreatePen(PS_SOLID, 2, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(BLACK);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}
bool Node::isIn(int x, int y)
{
	if( (x-this->x)*(x-this->x)+(y-this->y)*(y-this->y) <= r*r)
	{
		return true;
	}
	return false;
}

void Node::LDown()
{
	lclicked = true;
}
bool Node::LUp()
{
	if(lclicked)
	{
		lclicked = false;
		return true;
	}
	return false;
}

void Node::RDown()
{
	rclicked = true;
}
bool Node::RUp()
{
	if(rclicked)
	{
		rclicked = false;
		return true;
	}
	return false;
}