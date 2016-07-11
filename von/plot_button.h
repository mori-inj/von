#ifndef __PLOT_BUTTON__
#define __PLOT_BUTTON__

#include "node.h"


class PlotButton : public Node
{
private:
	int x, y,r;
	bool clicked;
public:
	PlotButton(int x, int y, int r);
	void print(HDC hdc);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif