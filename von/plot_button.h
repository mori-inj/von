#ifndef __PLOT_BUTTON__
#define __PLOT_BUTTON__

#include "button.h"


class PlotButton : public Button
{
private:
	int x, y,r;
	bool clicked;
public:
	bool plot_mode;
	PlotButton(int x, int y, int r);
	void print(HDC hdc);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif