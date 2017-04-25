#include "temp_weight.h"
#include "print.h"
#include <math.h>
using namespace std;

TempWeight::TempWeight(Weight& weight) : Weight(weight)
{
}

TempWeight::TempWeight(Node* src) : Weight(src)
{
	src_pos = src->getXY();
}

void TempWeight::setSXY(int x, int y)
{
	src_pos = Coord<int>(x,y);
}


void TempWeight::setDXY(int x, int y)
{
	dst_pos = Coord<int>(x,y);
}


void TempWeight::print(HDC hdc)
{
	HPEN hPen,oldPen;

	hPen = CreatePen(PS_SOLID, 2, WHITE);
	oldPen = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, src_pos.x, src_pos.y, NULL); 
	LineTo(hdc, dst_pos.x, dst_pos.y); 
	
	int dx = src_pos.x - dst_pos.x;
	int dy = src_pos.y - dst_pos.y;
	long double d = (int)sqrt(dx*dx+dy*dy);
	if(d>=1)
	{	
		int arrow_endx = (int)dst_pos.x + (10)*dx/d;
		int arrow_endy = (int)dst_pos.y + (10)*dy/d;
		int left_halfx = (int)arrow_endx + 7*dy/d;
		int left_halfy = (int)arrow_endy - 7*dx/d;
		int right_halfx = (int)arrow_endx - 7*dy/d;
		int right_halfy = (int)arrow_endy + 7*dx/d;

		MoveToEx(hdc, left_halfx, left_halfy, NULL);
		LineTo(hdc, dst_pos.x, dst_pos.y);
		MoveToEx(hdc, right_halfx, right_halfy, NULL);
		LineTo(hdc, dst_pos.x, dst_pos.y);	
	}

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}