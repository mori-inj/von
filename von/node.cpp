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
		EllipseLine(hdc, pos.x, pos.y, r, size, RED, BLACK);
	}
	else if (plot_output)
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, GREEN, BLACK);
	}
	else if (plot_border_output)
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, CYAN, BLACK);
	}
	else if (input_node)
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, YELLOW, BLACK);
	}
	else
	{
		EllipseLine(hdc, pos.x, pos.y, r, size, white, BLACK);
	}
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