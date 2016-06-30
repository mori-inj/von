#include "weight.h"
#include "print.h"
#include <math.h>
#include <iostream>
using namespace std;

Weight::Weight(Weight& weight)
{
	this->w =  getW();
	this->src = weight.getSrc();
	this->dst = weight.getDst();
	this->srcx = weight.getSX();
	this->srcy = weight.getSY();
	this->dstx = weight.getDX();
	this->dsty = weight.getDY();
}

Weight::Weight(Node* src)
{
	this->src = src;
	/*int dx = abs(src->getX() - dst->getX());
	int dy = abs(src->getY() - dst->getY());
	long double d = (int)sqrt(dx*dx+dy*dy);
	int snx = src->getR()*(dx/d);
	int sny = src->getR()*(dy/d);*/

	srcx = src->getX();//+snx;
	srcy = src->getY();//+sny;
}
Weight::Weight(Node* src, Node* dst)
{
	this->src = src;
	this->dst = dst;

	/*int dx = abs(src->getX() - dst->getX());
	int dy = abs(src->getY() - dst->getY());
	long double d = (int)sqrt(dx*dx+dy*dy);
	int snx = src->getR()*(dx/d);
	int sny = src->getR()*(dy/d);
	int dnx = dst->getR()*(dx/d);
	int dny = dst->getR()*(dy/d);*/

	srcx = src->getX();//+snx;
	srcy = src->getY();//+sny;

	dstx = dst->getX();//-dnx;
	dsty = dst->getY();//-dny;
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

int Weight::getSX()
{
	return srcx;
}
int Weight::getSY()
{
	return srcy;
}
int Weight::getDX()
{
	return dstx;
}
int Weight::getDY()
{
	return dsty;
}

void Weight::setDst(Node& dst)
{
	this->dst = &dst;

	/*int dx = abs(src->getX() - dst.getX());
	int dy = abs(src->getY() - dst.getY());
	long double d = (int)sqrt(dx*dx+dy*dy);
	int dnx = dst.getR()*(dx/d);
	int dny = dst.getR()*(dy/d);*/

	dstx = dst.getX();//-dnx;
	dsty = dst.getY();//-dny;
}

void Weight::setW(long double w)
{
	this->w = w;
}

void Weight::setSXSY(int x, int y)
{
	srcx = x;
	srcy = y;
}

void Weight::setDXDY(int x, int y)
{
	dstx = x;
	dsty = y;
}

void Weight::print(HDC hdc)
{
	HPEN hPen,oldPen;

	hPen = CreatePen(PS_SOLID, 2, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);

	cout << dstx << " " << dsty << " " << srcx << " " << srcy << endl;
	
	MoveToEx(hdc,srcx,srcy,NULL); 
	LineTo(hdc,dstx,dsty); 

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}
bool Weight::isIn(int x, int y)
{
	return false;
}