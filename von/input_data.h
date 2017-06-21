#ifndef __INPUT_DATA__
#define __INPUT_DATA__

#include "button.h"


class InputData : public Button
{
private:
	Coord<long double> pos;
	int class_id;
	bool clicked;
	COLORREF color;
public:
	bool selected;
	InputData(long double x, long double y, int class_id);
	Coord<long double> getXY();
	int get_class();
	void print(HDC hdc, int zoom);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif