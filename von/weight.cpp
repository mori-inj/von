#include "weight.h"
#include "print.h"
#include <math.h>

Weight::Weight(Node& src, Node& dst)
{
	this->src = src;
	int dx = abs(src.getX() - dst.getX());
	int dy = abs(src.getY() - dst.getY());
	long double d = (int)sqrt(dx*dx+dy*dy);
	int snx = src.getR()*(dx/d);
	int sny = src.getR()*(dy/d);

	sx = src.getX()+snx;
	sy = src.getY()+sny;
}
Weight::Weight(Node& src, Node& dst)
{
	this->src = src;
	this->dst = dst;

	int dx = abs(src.getX() - dst.getX());
	int dy = abs(src.getY() - dst.getY());
	long double d = (int)sqrt(dx*dx+dy*dy);
	int snx = src.getR()*(dx/d);
	int sny = src.getR()*(dy/d);
	int dnx = dst.getR()*(dx/d);
	int dny = dst.getR()*(dy/d);

	sx = src.getX()+snx;
	sy = src.getY()+sny;

	dx = dst.getX()-dnx;
	dy = dst.getY()-dny;
}

void Weight::setDst(Node& dst)
{
	this->dst = dst;

	int dx = abs(src.getX() - dst.getX());
	int dy = abs(src.getY() - dst.getY());
	long double d = (int)sqrt(dx*dx+dy*dy);
	int dnx = dst.getR()*(dx/d);
	int dny = dst.getR()*(dy/d);

	dx = dst.getX()-dnx;
	dy = dst.getY()-dny;
}

void Weight::setW(long double w)
{
	this->w = w;
}
void Weight::setDXDY(int x, int y)
{
	this->dx = x;
	this->dy = y;
}
void Weight::print(HDC hdc)
{
	HPEN hPen,oldPen;

	hPen = CreatePen(PS_SOLID, 2, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);

	
	MoveToEx(hdc,sx,sy,NULL); 
	LineTo(hdc,dx,dy); 

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}
bool Weight::isIn(int x, int y)
{
	return false;
}