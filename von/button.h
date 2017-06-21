#ifndef __BUTTON__
#define __BUTTON__


#include <Windows.h>
#include "coord.h"

class Button
{
protected:
	bool lclicked, rclicked;
	Coord<int> pos;
	int r;
public:
	Button(){};
	Button(int x, int y);
	Button(int x, int y, int r);

	bool isIn(int x, int y);
	Coord<int> getXY();
	int getR();
	void setXY(int x, int y);

	void LDown();
	bool LUp();
	void RDown();
	bool RUp();

	void print(HDC hdc);
};


#endif