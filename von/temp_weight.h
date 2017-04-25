#ifndef __TEMP_WEIGHT__
#define __TMEP_WEIGHT__

#include "weight.h"

class TempWeight : public Weight
{
private:
	Coord<int> src_pos;
	Coord<int> dst_pos;
public:
	TempWeight(){};
	TempWeight(Node* src);
	TempWeight(Weight& weight);

	void setSXY(int x, int y);
	void setDXY(int x, int y);
	void print(HDC hdc);
};

#endif