#ifndef __WEIGHT__
#define __WEIGHT__

#include <Windows.h>
#include "node.h"

typedef int NodeIdx;

class Weight
{
protected:
	long double w;
	Node* src;
	Node* dst;
public:
	bool plot_mode;
	Weight(){};
	Weight(Node* src);
	Weight(Weight& weight);
	Weight(Node* src, Node* dst);
	Node* getSrc();
	Node* getDst();
	long double getW();
	void setDst(Node& dst);
	void setW(long double w);
	void print(HDC hdc);
	bool isIn(int x, int y);
	Coord<int> getSXY();
	Coord<int> getDXY();
};

#endif