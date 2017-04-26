#ifndef __OUTPUT_BUTTON__
#define __OUTPUT_BUTTON__

#include "button.h"


class OutputButton : public Button
{
private:
	int x, y,r;
	bool clicked;
public:
	OutputButton(int x, int y, int r);
	void print(HDC hdc);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif