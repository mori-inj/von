#ifndef __NODE_BUTTON__
#define __NODE_BUTTON__

#include "button.h"


class NodeButton : public Button
{
private:
	int x, y,r;
	bool clicked;
public:
	NodeButton(int x, int y, int r);
	void print(HDC hdc);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif