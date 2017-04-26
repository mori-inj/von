#include "node.h"
#include "weight.h"
#include "print.h"
#include "function.h"


Node::Node(int x, int y) : Button(x,y)
{
	bias = 0.5;
	plot_mode = false;
	input_node = true;
	plot_input = false;
	plot_output = false;
	plot_border_output = false;
	is_selected_left = false;
	is_selected_right = false;
}

Node::Node(Node* node)
{
	pos = node->getXY();
	r = node->getR();
	pos.x += r*2;
	pos.y += r*2;
	input = node->get_input();
	bias = node->get_bias();
	plot_mode = node->plot_mode;
	input_node = node->input_node;
	plot_input = node -> plot_input;
	plot_output = node -> plot_output;
	plot_border_output = node -> plot_border_output;
	is_selected_left = node -> is_selected_left;
	is_selected_right = node -> is_selected_right;

	for(int i=0; i<(int)node->weight_list.size(); i++)
	{
		weight_list.push_back(new Weight(node->weight_list[i]->getSrc(), this));
	}
}

void Node::printWeight(HDC hdc)
{
	for(int i=0; i<(int)weight_list.size(); i++)
		weight_list[i]->print(hdc);
}
void Node::print(HDC hdc)
{
	int size;
	COLORREF white;
	if (plot_mode)
	{
		size = 5;
		white = BRIGHTWHITE;
	}
	else
	{
		size = 1;
		white = WHITE;
	}

	if (plot_input)
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, RED, BLACK, is_selected_left, is_selected_right);
	}
	else if (plot_output)
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, GREEN, BLACK, is_selected_left, is_selected_right);
	}
	else if (plot_border_output)
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, CYAN, BLACK, is_selected_left, is_selected_right);
	}
	else if (input_node)
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, YELLOW, BLACK, is_selected_left, is_selected_right);
	}
	else
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, white, BLACK, is_selected_left, is_selected_right);
	}
}


long double Node::get_bias()
{
	return bias;
}

long double Node::get_input()
{
	return input;
}

void Node::set_input(long double input)
{
	this->input = input;
}

long double Node::get_output()
{
	long double sum = 0;
	if(weight_list.size()==0)
		return this->input;

	for(int i=0; i<(int)weight_list.size(); i++)
	{
		sum += ( weight_list[i]->getSrc()->get_output() ) * ( weight_list[i]->getW() );
	}
	sum += bias;
	return sigmoid(10,sum);
}