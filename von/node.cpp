#include "node.h"
#include "weight.h"
#include "print.h"
#include "function.h"


Node::Node(int x, int y) : Button(x,y)
{
	input_node = true;
}


void Node::printWeight(HDC hdc)
{
	for(int i=0; i<weight_list.size(); i++)
		weight_list[i]->print(hdc);
}
void Node::print(HDC hdc)
{
	EllipseLine(hdc, pos.x, pos.y, r, 1, input_node?YELLOW:WHITE, BLACK);
}


void Node::get_input(long double input)
{
	this->input = input;
}

long double Node::get_output()
{
	long double sum = 0;
	for(int i=0; i<weight_list.size(); i++)
	{
		sum += ( weight_list[i]->getSrc()->get_output() ) * ( weight_list[i]->getW() );
	}

	return sigmoid(sum);
}