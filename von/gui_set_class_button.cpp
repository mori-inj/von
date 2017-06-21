#include "gui_set_class_button.h"
#include "print.h"


GUISetClassButton::GUISetClassButton(int x, int y, int r, int class_id, COLORREF color, bool selected)
{
	this->pos.x = x;
	this->pos.y = y;
	this->r = r;
	this->class_id = class_id;
	this->color = color;
	this->selected = selected;
}

void GUISetClassButton::print(HDC hdc)
{
	HPEN hPen,oldPen;
	HBRUSH hBrush, oldBrush;

	if(selected)
		EllipseLine(hdc, pos.x,pos.y, r, 3, BRIGHTWHITE, color);
	else
		EllipseLine(hdc, pos.x,pos.y, r, 1, WHITE, color);

	SetTextColor(hdc, BLACK);
	SetBkColor(hdc, color);
		
	WCHAR str[10] = {};
	RECT rect;
	rect.top = pos.y - r + 2;
	rect.bottom = pos.y + r;
	rect.left = pos.x - r;
	rect.right = pos.x + r;
	wsprintf(str, L"%d",class_id);
	DrawText(hdc, str, wcslen(str), &rect, DT_CENTER | DT_VCENTER);
}
