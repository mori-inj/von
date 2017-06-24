#include "gui.h"
#include "print.h"
#include <stdio.h>//

GUI::GUI() : node_add_button(35,35,25) , plot_button(100,35,25) , input_button(165, 35, 25) , output_button(230, 35, 25)
{
	state = normal;
	model = Model();
	is_any_selected_left = is_any_selected_right = false;
	plot_input_count = 0;
	shift_down = false;
}

void GUI::print(HDC MemDC)
{
	node_add_button.print(MemDC);
	plot_button.print(MemDC);
	input_button.print(MemDC);
	output_button.print(MemDC);

	if(state == weight_add_mouse_move) {
		temp_weight -> print(MemDC);
	}

	model.print(MemDC);

	if(state == left_box_drag) {
		DrawRectangle(MemDC, &select_box_left, 2, WHITE);
	}

	if(state == right_box_drag) {
		DrawRectangle(MemDC, &select_box_right, 2, RED);
	}

}

void GUI::LDown(int x, int y)
{
	switch(state)
	{
		case normal:
		{
			if(node_add_button.isIn(x,y)) {
				state = node_add_button_LDown;
				printf("node_add_ldown %d\n",state); fflush(stdout);
			} else if(plot_button.isIn(x,y)) {
				state = plot_button_LDown;
				printf("plot_ldown %d\n",state); fflush(stdout);
			} else if(input_button.isIn(x,y)) {
				state = input_button_LDown;
				printf("input_ldown %d\n",state); fflush(stdout);
			} else if(output_button.isIn(x,y)) {
				state = output_button_LDown;
				printf("output_ldown %d\n",state); fflush(stdout);
			} else {
				int idx = model.get_idx_of_clicked_node(x, y);
				if (idx != -1) {
					node_idx = idx;
					state = node_LDown;
				} else {
					select_list_left.clear();
					is_any_selected_left = false;
					select_box_left.top = select_box_left.bottom = y;
					select_box_left.left = select_box_left.right = x;
					model.clear_selected_left();

					state = left_box_drag;
				}
			}
			break;
		}
		case node_add_mouse_move:
		{
			state = normal;
			break;
		}
		case weight_add_mouse_move:
		{
			int idx = model.get_idx_of_clicked_node(x, y);
			if (idx != -1) {
				Node* src = temp_weight->getSrc();
				Node* dst = model.get_node_by_idx(idx);
				if(!model.check_weight_exists(src, dst)) {
					model.update_weight_set(src, dst);
					temp_weight -> setDst(*dst);
					Weight* weight = new Weight(*temp_weight);
					delete(temp_weight);
					dst->input_weight_list.push_back(weight);
					src->output_weight_list.push_back(weight);
					dst->input_node = false;
				}
			}
			state = normal;
			break;
		}
		default:
		{
		}
	}
}

void GUI::LUp(int x, int y)
{
	switch(state)
	{
		case node_add_button_LDown:
		{
			if(node_add_button.isIn(x,y)) {
				printf("node_add_worked %d\n",state); fflush(stdout);
				temp_node = new Node(x,y, model.get_node_num());
				model.add_node(temp_node);
				state = node_add_mouse_move;
			} else {
				state = normal;
			}
			break;
		}
		case plot_button_LDown:
		{
			if(plot_button.isIn(x,y)) {
				printf("plot_worked %d\n",state); fflush(stdout);
			}
			state = normal;
			break;
		}
		case input_button_LDown:
		{
			if(input_button.isIn(x,y)) {
				printf("input_worked %d\n",state); fflush(stdout);
			}
			state = normal;
			break;
		}
		case output_button_LDown:
		{
			if(output_button.isIn(x,y)) {
				printf("output_worked %d\n",state); fflush(stdout);
			}
			state = normal;
			break;
		}
		case node_LDown:
		{
			temp_weight = new TempWeight(model.get_node_by_idx(node_idx));
			temp_weight -> setDXY(x,y);
			state = weight_add_mouse_move;
			break;
		}
		case node_drag:
		{
			state = normal;
			break;
		}
		case left_box_drag:
		{
			end_box_drag();
			state = normal;
			break;
		}
		default:
		{
		}
	}
}

void GUI::RDown(int x, int y, HWND* hWnd)
{
	switch(state)
	{
		case normal:
		{
			if(node_add_button.isIn(x,y) || plot_button.isIn(x,y)
				|| input_button.isIn(x,y) || output_button.isIn(x,y)) {
					break;
			} else {
				int idx = model.get_idx_of_clicked_node(x, y);
				if (idx != -1) {
					node_idx = idx;

					HMENU hPopupMenu = CreatePopupMenu();
					RECT rect;
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_INPUT, (LPCWSTR)L"Plot as input");
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_OUTPUT, (LPCWSTR)L"Plot as output");
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_BORDER_OUTPUT, (LPCWSTR)L"Plot as border output");
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_RESET, (LPCWSTR)L"Reset");
					if(GetWindowRect(*hWnd,(LPRECT)&rect))
						TrackPopupMenu(hPopupMenu, 
							TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, 
							x+rect.left+8, y+rect.top+29, 0, 
							*hWnd, NULL);

				} else {
					select_list_right.clear();
					is_any_selected_right = false;
					select_box_right.top = select_box_right.bottom = y;
					select_box_right.left = select_box_right.right = x;
					model.clear_selected_right();

					state = right_box_drag;
				}
			}
			break;
		}
		default:
		{
		}
	}
}

void GUI::RUp(int x, int y)
{
	switch(state)
	{
		case right_box_drag:
		{
			end_box_drag();
			state = normal;
			break;
		}
		default:
		{
		}
	}
}

void GUI::mouse_move(int x, int y)
{
	switch(state)
	{
		case node_add_mouse_move:
		{
			temp_node -> setXY(x,y);
			break;
		}
		case node_LDown:
		case node_drag:
		{
			state = node_drag;
			if( x<=0+MARGIN || y<=0+MARGIN || x>=WIDTH-1-MARGIN-30 || y>=HEIGHT-1-MARGIN-50 ){
				ReleaseCapture();
				state = normal;
				break;
			}
			if(is_any_selected_left && model.get_is_selected_left_by_idx(node_idx)) {
				Coord<int> clicked = model.get_node_by_idx(node_idx)->getXY();
				for(auto left : select_list_left) {
					Coord<int> original = left->getXY();
					left->setXY(original.x + (x - clicked.x), original.y + (y - clicked.y));
				}
			} else {
				model.setXY_of_idx(x,y,node_idx);
			}
			break;
		}
		case weight_add_mouse_move:
		{
			temp_weight -> setDXY(x,y);
			break;
		}
		case left_box_drag:
		{
			select_box_left.bottom = y;
			select_box_left.right = x;
			break;
		}
		case right_box_drag:
		{
			select_box_right.bottom = y;
			select_box_right.right = x;
			break;
		}
		default:
		{
		}
	}
}

bool isNodeInBox(LPRECT select_box, Node* node)
{
	Coord<int> pos = node->getXY();
	int r = node->getR();
	int left = min(select_box->left, select_box->right);
	int right = max(select_box->left, select_box->right);
	int top = min(select_box->top, select_box->bottom);
	int bottom = max(select_box->top, select_box->bottom);

	if(left <= pos.x - r && pos.x + r <= right
		&& top <= pos.y - r && pos.y + r <= bottom)
		return true;
	return false;
}

void GUI::end_box_drag()
{
	if(state==left_box_drag)
	{
		for(auto node = model.get_first_node_iter(); node != model.get_last_node_iter(); node++)
		{
			if(isNodeInBox(&select_box_left, *node))
			{
				(*node)->is_selected_left = true;
				is_any_selected_left = true;
				select_list_left.insert(*node);
			}
		}
	}
	else if(state==right_box_drag)
	{
		for(auto node = model.get_first_node_iter(); node != model.get_last_node_iter(); node++)
		{
			if(isNodeInBox(&select_box_right, *node))
			{
				(*node)->is_selected_right = true;
				is_any_selected_right = true;
				select_list_right.insert(*node);
			}
		}
	}

	if(intersect(select_list_left.begin(), select_list_left.end(),
				select_list_right.begin(), select_list_right.end()))
	{
		is_any_selected_left = false;
		is_any_selected_right = false;
		select_list_left.clear();
		select_list_right.clear();
		model.clear_selected_left();
		model.clear_selected_right();
		return;
	}

	if(is_any_selected_right && is_any_selected_left)
	{
		for(auto right : select_list_right)
		{
			for(auto left : select_list_left)
			{
				if(!model.check_weight_exists(left, right))
				{
					model.update_weight_set(left, right);
					Weight* weight = new Weight(left, right);
					right->input_weight_list.push_back(weight);
					left->output_weight_list.push_back(weight);
					right->input_node = false;
				}
			}
		}

		is_any_selected_left = false;
		is_any_selected_right = false;
		select_list_left.clear();
		select_list_right.clear();
		model.clear_selected_left();
		model.clear_selected_right();
	}
}


void GUI::command(int msg)
{
	switch(msg)
	{
		case IDM_PLOT_INPUT:
		{
			if (plot_input_count < 2)
			{
				Node* node = model.get_node_by_idx(node_idx);
				if(node->plot_output || node ->plot_border_output)
				{
					plot_out_list.erase(node);
				}

				node -> plot_input = true;
				node -> plot_border_output = false;
				node -> plot_output = false;
				plot_input_count++;
				plot_in_list.insert(node);
			}
			break;
		}
		case IDM_PLOT_OUTPUT:
		{
			Node* node = model.get_node_by_idx(node_idx);
			if(node -> plot_input)
			{
				plot_input_count--;
				plot_in_list.erase(node);
			}
			if(node ->plot_border_output)
			{
				plot_out_list.erase(node);
			}

			node -> plot_input = false;
			node -> plot_border_output = false;
			node -> plot_output = true;

			plot_out_list.insert(node);			
			break;
		}
		case IDM_PLOT_BORDER_OUTPUT:
		{
			Node* node = model.get_node_by_idx(node_idx);
			if(node -> plot_input)
			{
				plot_input_count--;
				plot_in_list.erase(node);
			}
			if(node -> plot_output)
			{
				plot_out_list.erase(node);
			}

			node -> plot_input = false;
			node -> plot_output = false;
			node -> plot_border_output = true;

			plot_out_list.insert(node);
			break;
		}
		case IDM_RESET:
		{
			Node* node = model.get_node_by_idx(node_idx);
			if(node -> plot_input)
			{
				plot_input_count--;
				plot_in_list.erase(node);
			}
			if(node -> plot_output || node -> plot_border_output)
			{
				plot_out_list.erase(node);
			}

			node -> plot_input = false;
			node -> plot_output = false;
			node -> plot_border_output = false;
			break;
		}
	}
}

void GUI::keyboard_down(string input)
{
	if(input.compare("all")==0){
		//nothing
	} else if(input.compare("copy")==0) {
		for(auto left : select_list_left) {
			copy_list.push_back(left);
		}
	} else if(input.compare("paste")==0) {
		vector<pair<Node*, Node*>> copied_list;
		for(auto original_node : copy_list) {
			Node* copied_node = new Node(original_node, model.get_node_num());
			model.add_node(copied_node);
			select_list_left.erase(original_node);
			original_node -> is_selected_left = false;
			select_list_left.insert(copied_node);
			if(shift_down) { 
				copied_list.push_back(make_pair(original_node, copied_node));
			} else {
				copied_list.push_back(make_pair(new Node(*original_node), copied_node));
			}
		}
		for(auto p : copied_list) {
			for(auto w : p.first -> input_weight_list) {
				Node* src = w -> getSrc();
				Weight* weight = new Weight(src, p.second);
				src -> output_weight_list.push_back(weight);
				p.second -> input_weight_list.push_back(weight);
				model.update_weight_set(src, p.second);
			}
			for(auto w : p.first -> output_weight_list) {
				Node* dst = w -> getDst();
				Weight* weight = new Weight(p.second, dst);
				dst -> input_weight_list.push_back(weight);
				p.second -> output_weight_list.push_back(weight);
				model.update_weight_set(p.second, dst);
			}
		}
		copy_list.clear();
	} else if(input.compare("cut")==0) {
		//nothing
	} else if(input.compare("shift")==0) {
		shift_down = true;
	}

}

void GUI::keyboard_up(string input)
{
	if(input.compare("delete")==0) {
		if(is_any_selected_left){
			for(int i=0; i<(int)node_list.size(); i++)
			{
				for(int j=0; j<(int)node_list[i]->weight_list.size(); j++)
				{
					if(node_list[i]->weight_list[j]->getSrc()->is_selected_left)
					{
						node_list[i]->weight_list.erase(node_list[i]->weight_list.begin()+j);
						j--;
					}
				}
			}
			for(int i=0; i<(int)node_list.size(); i++)
			{
				if(node_list[i]->is_selected_left)
				{
					if(node_list[i]->plot_input)
					{
						plot_in_list.erase(node_list[i]);
						plot_input_count--;
					}
					if(node_list[i]->plot_output || node_list[i]->plot_border_output)
					{
						plot_out_list.erase(node_list[i]);
					}
								
					node_list.erase(node_list.begin()+i);
					i--;
				}
			}
		}
	} else if(input.compare("shift")==0) {
		shift_down = false;
	}
}