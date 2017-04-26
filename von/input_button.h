#ifndef __INPUT_BUTTON__
#define __INPUT_BUTTON__

#include "button.h"


class InputButton : public Button
{
private:
	int x, y,r;
	bool clicked;
public:
	InputButton(int x, int y, int r);
	void print(HDC hdc);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif