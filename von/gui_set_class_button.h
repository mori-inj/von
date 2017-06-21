#ifndef __GUI_SET_CLASS_BUTTON__
#define __GUI_SET_CLASS_BUTTON__

#include "button.h"


class GUISetClassButton : public Button
{
private:
	int class_id;
	bool clicked;
	COLORREF color;
public:
	bool selected;
	GUISetClassButton(int x, int y, int r, int class_id, COLORREF color, bool selected);
	void print(HDC hdc);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif