#ifndef __WEIGHT__
#define __WEIGHT__

#include <Windows.h>
#include "node.h"

typedef int NodeIdx;

class Weight
{
private:
	long double w;
	Node* src;
	Node* dst;
	int srcx,srcy,dstx,dsty;
public:
	Weight(){};
	Weight(Node* src);
	Weight(Weight& weight);
	Weight(Node* src, Node* dst);
	Node* getSrc();
	Node* getDst();
	long double getW();
	void setDst(Node& dst);
	void setSXSY(int x, int y);
	void setDXDY(int x, int y);
	void setW(long double w);
	void print(HDC hdc);
	bool isIn(int x, int y);
	int getSX();
	int getSY();
	int getDX();
	int getDY();
};

#endif