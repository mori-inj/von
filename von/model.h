#ifndef __MODEL__
#define __MODEL__

#include <vector>
#include <set>
#include "node.h"

using namespace std;

class Model
{
private:
	vector<Node*> node_list;
	set<pair<Node*, Node*> > weight_set;
	vector<Weight*> weight_list;
public:
	Model();
	~Model();
	int get_node_num() { return (int)node_list.size(); }
	void add_node(Node* node);
	Node* get_node_by_idx(int idx);
	bool check_weight_exists(Node* a, Node* b);
	void update_weight_set(Weight* w);
	void remove_weight_set(Weight* w);
	vector<Node*>::iterator get_first_node_iter();
	vector<Node*>::iterator get_last_node_iter();
	void erase_node(Node* node);
	void reindex();

	void setXY_of_idx(int x, int y, int idx); // #GUI
	int get_idx_of_clicked_node(int x, int y); // #GUI
	bool get_is_selected_left_by_idx(int idx); // #GUI
	void clear_selected_left(); // #GUI
	void clear_selected_right(); // #GUI
	void print(HDC MemDC); // #GUI
	void clear_plot_mode();
	int count_input_node();
};

#endif
