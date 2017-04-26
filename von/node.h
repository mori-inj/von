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
	long double bias;
public:
	bool plot_mode;
	bool input_node;
	bool plot_input;
	bool plot_output;
	bool plot_border_output;

	//list of weights whose dst is this node
	vector<Weight*> weight_list;

	Node(){};
	Node(int x, int y);
	void print(HDC hdc);
	void printWeight(HDC hdc);
	void set_input(long double input);
	long double get_output();
};

#endif
