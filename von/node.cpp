#include "node.h"
#include "weight.h"
#include "print.h"
#include "function.h"

extern int OUTPUT_CNT;

Node::Node(int x, int y, int idx) : Button(x,y)
{
	bias = 0.5;
	this->idx = idx;
	plot_mode = false;
	input_node = true;
	plot_input = false;
	plot_output = false;
	plot_border_output = false;
	is_selected_left = false;
	is_selected_right = false;

	target_output = 0;
}

Node::Node(Node* node, int idx)
{
	this->idx = idx;
	pos = node->getXY();
	r = node->getR();
	pos.x += r*2;
	pos.y += r*2;
	input = node->get_input();
	bias = node->get_bias();
	
	input_node = node->input_node;
	
	plot_mode = false;//node->plot_mode;
	plot_input = false;//node -> plot_input;
	plot_output = false;//node -> plot_output;
	plot_border_output = false;//node -> plot_border_output;
	
	is_selected_left = node -> is_selected_left;
	is_selected_right = node -> is_selected_right;

	target_output = 0;
}

void Node::print_weight(HDC hdc)
{
	for(int i=0; i<(int)input_weight_list.size(); i++)
		input_weight_list[i]->print(hdc);
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

	//HPEN hPen,oldPen;
	//hPen = CreatePen(PS_SOLID, 2, white);
	//oldPen = (HPEN)SelectObject(hdc, hPen);
	if(is_selected_left)
	{
		SetTextColor(hdc, BLACK);
		SetBkColor(hdc, WHITE);
	}
	else if(is_selected_right)
	{
		SetTextColor(hdc, BLACK);
		SetBkColor(hdc, RED);
	}
	else 
	{
		SetTextColor(hdc, WHITE);
		SetBkColor(hdc, BLACK);
		
	}
	WCHAR str[10] = {};
	RECT rect;
	rect.top = pos.y - r/2;
	rect.bottom = pos.y + r/2;
	rect.left = pos.x - r/2;
	rect.right = pos.x + r/2;
	wsprintf(str, L"%d",idx);
	DrawText(hdc, str, (int)wcslen(str), &rect, DT_CENTER | DT_VCENTER);
	//SelectObject(hdc, oldPen);
	//DeleteObject(hPen);
}

void Node::set_idx(int idx)
{
	this->idx = idx;
}

int Node::get_idx()
{
	return idx;
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

long double Node::get_linear_output()
{
	long double sum = 0;
	if(input_weight_list.size()==0)
		return this->input;

	for(int i=0; i<(int)input_weight_list.size(); i++)
	{
		sum += ( input_weight_list[i]->getSrc()->get_output() ) * ( input_weight_list[i]->getW() );
	}
	sum += bias;

	return sum;
}

long double Node::get_output() //TODO: needs to be cached
{
	
	OUTPUT_CNT++;
	long double sum = get_linear_output();
	if(input_weight_list.size()==0)
		return sum;

	return sigmoid(10, sum);
}


void Node::set_target_output(long double x)
{
	target_output = x;
}

long double Node::get_delta()
{
	return delta;
}

long double Node::calc_delta() //does not check get_delta is valid
{
	if(output_weight_list.empty()) {
		delta = get_output() - target_output;
	} else {
		delta = 0;
		for(auto it : output_weight_list) {
			delta += it->getW() * it->getDst()->calc_delta();
		}
		delta *= deriv_sigmoid(10, get_linear_output());
	}
	return delta;
}

void Node::calc_grad()
{
	grad.clear();

	long double output = get_output();
	for(auto it : output_weight_list) {
		grad.push_back(output * it->getDst()->get_delta());
	}

	/*for(auto it : input_weight_list) {
		grad.push_back(it->getSrc()->get_output() * delta);
	}*/
}

void Node::update_weight(long double learning_rate)
{
	for(int i=0; i<(int)output_weight_list.size(); i++) {
		Weight* w = output_weight_list[i];
		long double weight = w->getW();
		weight = weight - learning_rate * grad[i];
		w->setW(weight);
	}
	bias -= learning_rate * delta;
}