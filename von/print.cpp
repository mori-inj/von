#include <Windows.h>

void EllipseLine(HDC hdc, int x, int y, int r, int d, COLORREF color, COLORREF bg)
{
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;
	hPen = CreatePen(PS_SOLID, d, color);
	oldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(bg);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc,x-r,y-r,x+r,y+r);
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}