#include "model.h"
#include "weight.h"

Model::Model()
{
}
void Model::add_node(Node* node)
{
	node_list.push_back(node);
	/*printf("===========\nnode list:\n");
	for(int i=0; i<(int)node_list.size(); i++)
	{
		printf("%d\n",i);
		for(int j=0; j<(int)node_list[i]->input_weight_list.size(); j++) {
			printf("  %d -> %d\n", node_list[i]->input_weight_list[j]->getSrc()->idx, i); fflush(stdout);
		}
		for(int j=0; j<(int)node_list[i]->output_weight_list.size(); j++) {
			printf("  %d -> %d\n", i, node_list[i]->output_weight_list[j]->getDst()->idx); fflush(stdout);
		}
	}
	printf("============\n\n"); fflush(stdout);

	printf("===========\nweight set:\n");
	for(auto w : weight_set) {
		printf("%d -> %d\n",w.first->idx,w.second->idx); fflush(stdout);
	}
	printf("============\n\n"); fflush(stdout);*/
}
Node* Model::get_node_by_idx(int idx)
{
	return node_list[idx];
}
vector<Node*>::iterator Model::get_first_node_iter()
{
	return node_list.begin();
}
vector<Node*>::iterator Model::get_last_node_iter()
{
	return node_list.end();
}
bool Model::check_weight_exists(Node* a, Node* b)
{
	if(weight_set.find(make_pair(a,b)) != weight_set.end())
		return true;
	return false;
}
void Model::update_weight_set(Node* a, Node* b)
{
	weight_set.insert(make_pair(a,b));
}





void Model::setXY_of_idx(int x, int y, int idx)
{
	node_list[idx] -> setXY(x,y);
}
int Model::get_idx_of_clicked_node(int x, int y)
{
	for(int i=0; i<node_list.size(); i++)
	{
		if(node_list[i]->isIn(x, y))
		{
			return node_list[i]->idx;
		}
	}
	return -1;
}
bool Model::get_is_selected_left_by_idx(int idx)
{
	return node_list[idx]->is_selected_left;
}
void Model::clear_selected_left()
{
	for(int i = 0; i<(int)node_list.size(); i++)
	{
		node_list[i]->is_selected_left = false;
	}
}
void Model::clear_selected_right()
{
	for(int i = 0; i<(int)node_list.size(); i++)
	{
		node_list[i]->is_selected_right = false;
	}
}
void Model::print(HDC MemDC)
{
	for(auto node : node_list) {
		node -> print_weight(MemDC);
	}
	for(auto node: node_list) {
		node -> print(MemDC);
	}
}