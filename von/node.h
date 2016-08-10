#ifndef __NODE__
#define __NODE__

#include <vector>
#include "button.h"

class Weight;

using namespace std;

class Node : public Button
{
private:
	int x, y,r;
	bool lclicked, rclicked;
	long double input;
public:
	bool input_node;
	vector<Weight*> weight_list;
	Node(){};
	Node(int x, int y);
	int getX();
	int getY();
	int getR();
	void setXY(int x, int y);
	void print(HDC hdc);
	void printWeight(HDC hdc);
	bool isIn(int x, int y);
	void LDown();
	bool LUp();
	void RDown();
	bool RUp();
	void get_input(long double input);
	long double get_output();
};

#endif
