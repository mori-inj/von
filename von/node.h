#ifndef __NODE__
#define __NODE__

#include <vector>
#include "button.h"
#include "weight.h"

using namespace std;

class Node : public Button
{
private:
	int x, y,r;
	bool lclicked, rclicked;
	vector<Weight*> weight_list;

public:
	Node(){};
	Node(int x, int y);
	int getX();
	int getY();
	int getR();
	void setXY(int x, int y);
	void print(HDC hdc);
	bool isIn(int x, int y);
	void LDown();
	bool LUp();
	void RDown();
	bool RUp();
};

#endif
