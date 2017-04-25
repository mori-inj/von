#include "weight.h"
#include "print.h"
#include <math.h>
using namespace std;

Weight::Weight(Weight& weight)
{
	this->w =  getW();
	this->src = weight.getSrc();
	this->dst = weight.getDst();
}

Weight::Weight(Node* src)
{
	this->src = src;
	this->dst = NULL;
}
Weight::Weight(Node* src, Node* dst)
{
	this->src = src;
	this->dst = dst;
}

Node* Weight::getSrc()
{
	return src;
}

Node* Weight::getDst()
{
	return dst;
}

long double Weight::getW()
{
	return w;
}

Coord<int> Weight::getSXY()
{
	return src->getXY();
}

Coord<int> Weight::getDXY()
{
	return dst->getXY();
}

void Weight::setDst(Node& dst)
{
	this->dst = &dst;
}

void Weight::setW(long double w)
{
	this->w = w;
}

void Weight::print(HDC hdc)
{
	HPEN hPen,oldPen;

	hPen = CreatePen(PS_SOLID, 2, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, src->getXY().x, src->getXY().y, NULL); 
	LineTo(hdc, dst->getXY().x, dst->getXY().y); 
	
	int dx = src->getXY().x - dst->getXY().x;
	int dy = src->getXY().y - dst->getXY().y;
	long double d = (int)sqrt(dx*dx+dy*dy);
	if(d>=1)
	{
		int arrow_endx = (int)dst->getXY().x + (10+dst->getR())*dx/d;
		int arrow_endy = (int)dst->getXY().y + (10+dst->getR())*dy/d;
		int left_halfx = (int)arrow_endx + 7*dy/d;
		int left_halfy = (int)arrow_endy - 7*dx/d;
		int right_halfx = (int)arrow_endx - 7*dy/d;
		int right_halfy = (int)arrow_endy + 7*dx/d;
	
		MoveToEx(hdc, left_halfx, left_halfy, NULL);
		LineTo(hdc,dst->getXY().x+(dst->getR()*dx/d),dst->getXY().y+(dst->getR()*dy/d));
		MoveToEx(hdc, right_halfx, right_halfy, NULL);
		LineTo(hdc,dst->getXY().x+(dst->getR()*dx/d),dst->getXY().y+(dst->getR()*dy/d));
	}

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}
bool Weight::isIn(int x, int y)
{
	return false;
}