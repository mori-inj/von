#ifndef __NODE__
#define __NODE__

#include <vector>
#include "button.h"

class Weight;

using namespace std;

class Node : public Button
{
private:
	long double input;
public:
	bool input_node;
	vector<Weight*> weight_list;

	Node(){};
	Node(int x, int y);
	void print(HDC hdc);
	void printWeight(HDC hdc);
	void get_input(long double input);
	long double get_output();
};

#endif
