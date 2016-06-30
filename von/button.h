#ifndef __BUTTON__
#define __BUTTON__


#include <Windows.h>

class Button
{
private:
	bool clicked;
public:
	void print(HDC hdc);
	bool isIn(int x, int y);
	void LDown();
	void LUp();
	void RDown();
	void RUp();
};


#endif