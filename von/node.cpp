#include "node.h"
#include "weight.h"
#include "print.h"
#include "function.h"


Node::Node(int x, int y)
{
	r = 25;
	input_node = true;
	this->x = x;
	this->y = y;
}

void Node::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Node::getX()
{
	return x;
}

int Node::getY()
{
	return y;
}

int Node::getR()
{
	return r;
}
void Node::printWeight(HDC hdc)
{
	for(int i=0; i<weight_list.size(); i++)
		weight_list[i]->print(hdc);
}
void Node::print(HDC hdc)
{
	EllipseLine(hdc, x,y,r,1,input_node?YELLOW:WHITE, BLACK);
}
bool Node::isIn(int x, int y)
{
	if( (x-this->x)*(x-this->x)+(y-this->y)*(y-this->y) <= r*r)
	{
		return true;
	}
	return false;
}

void Node::LDown()
{
	lclicked = true;
}
bool Node::LUp()
{
	if(lclicked)
	{
		lclicked = false;
		return true;
	}
	return false;
}

void Node::RDown()
{
	rclicked = true;
}
bool Node::RUp()
{
	if(rclicked)
	{
		rclicked = false;
		return true;
	}
	return false;
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