#ifndef __WEIGHT__
#define __WEIGHT__

#include <Windows.h>
#include "node.h"

typedef int NodeIdx;

class Weight
{
private:
	long double w;
	Node src;
	Node dst;
	int sx,sy,dx,dy;
public:
	Weight(Node& src);
	Weight(Node& src, Node& dst);
	void setDst(Node& dst);
	void setDXDY(int x, int y);
	void setW(long double w);
	void print(HDC hdc);
	bool isIn(int x, int y);
};

#endif