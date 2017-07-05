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
	long double target_output;
	long double bias;
	int idx;
	long double delta;
	vector<long double> grad;
public:
	bool plot_mode;
	bool input_node;
	bool plot_input;
	bool plot_output;
	bool plot_border_output;
	bool is_selected_left, is_selected_right;

	vector<Weight*> input_weight_list;
	vector<Weight*> output_weight_list;

	Node(){};
	Node(int x, int y, int idx);
	Node(Node* node, int idx);
	void print(HDC hdc);
	void print_weight(HDC hdc);
	void set_idx(int idx);
	int get_idx();
	long double get_bias();
	long double get_input();
	void set_input(long double input);
	long double get_linear_output();
	long double get_output();
	void set_target_output(long double x);
	long double get_delta();
	long double calc_delta();
	void calc_grad();
	void update_weight(long double learning_rate);
};

#endif
