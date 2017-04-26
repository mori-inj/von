#include "print.h"

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

void EllipseLine(HDC hdc, int x, int y, int r, int d, COLORREF color, COLORREF bg, bool is_selected_left, bool is_selected_right)
{
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;
	hPen = CreatePen(PS_SOLID, d, color);
	oldPen = (HPEN)SelectObject(hdc, hPen);
	if(is_selected_left)
		hBrush = CreateSolidBrush(WHITE);
	else if(is_selected_right)
		hBrush = CreateSolidBrush(RED);
	else
		hBrush = CreateSolidBrush(bg);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc,x-r,y-r,x+r,y+r);
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}


void DrawRectangle(HDC hdc, LPRECT rect, int d, COLORREF color)
{
	HPEN hPen, oldPen;
	hPen = CreatePen(PS_SOLID, d, color);
	oldPen = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, rect->left, rect->top, NULL); 
	LineTo(hdc, rect->right, rect->top);
	LineTo(hdc, rect->right, rect->bottom);
	LineTo(hdc, rect->left, rect->bottom);
	LineTo(hdc, rect->left, rect->top);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}