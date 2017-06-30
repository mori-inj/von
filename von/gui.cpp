#include <Windowsx.h>
#include "gui.h"
#include "print.h"
#include "gui_set_class_button.h"
#include <stdio.h>//
#include <assert.h>//

set<Node*> GUI::plot_in_list;
set<Node*> GUI::plot_out_list;
HWND GUI::plotWindowHwnd, GUI::inputWindowHwnd, GUI::inputGUIWindowHwnd, GUI::outputWindowHwnd;
HINSTANCE* GUI::g_hInst;
vector<InputData*> GUI::input_data_list;
set<InputData*> GUI::input_data_set[2];

bool is_node_in_box(LPRECT select_box, Node* node);
bool plot_verify(Node* idx, bool reset);
COLORREF color_add(COLORREF ac, COLORREF bc);
COLORREF color_scalar_mul(long double k, COLORREF ac);

GUI::GUI(HINSTANCE* g_hInst) : node_add_button(35,35,25) , plot_button(100,35,25) , input_button(165, 35, 25) , output_button(230, 35, 25)
{
	state = normal;
	model = Model();
	is_any_selected_left = is_any_selected_right = false;
	plot_input_count = 0;
	shift_down = false;
	this->g_hInst = g_hInst;
}

void GUI::print(HDC MemDC, HWND& hWnd)
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

	model.clear_plot_mode();
	bool ret = true;
	for(auto it : plot_out_list) {
		bool temp  = plot_verify(it, true);
		ret = ret && temp;
	}
	if(ret && plot_input_count == 2 && (int)plot_out_list.size()>0) {
		plot_button.plot_mode = true;
	} else {
		plot_button.plot_mode = false;
		if(plotWindowHwnd!=NULL) {
			SendMessage(plotWindowHwnd, WM_CLOSE, NULL, NULL);
		}
	}

	int cnt = model.count_input_node();
	if(cnt==2) {
		if(inputGUIWindowHwnd==NULL && inputWindowHwnd!=NULL) {
			RECT temp;
			GetWindowRect(hWnd, &temp);
			SendMessage(inputGUIWindowHwnd, WM_CLOSE, NULL, NULL);
			inputGUIWindowHwnd = CreateWindow(
						L"InputGUI",
						L"Input 2D Data",
						WS_BORDER | WS_CHILD | WS_POPUPWINDOW | WS_OVERLAPPEDWINDOW,
						temp.left - 600,
						temp.top + 300,
						300,
						300,
						hWnd,
						(HMENU)0,
						*g_hInst,
						NULL
						);
			ShowWindow(inputGUIWindowHwnd, SW_SHOW);
		}
	}

}

void GUI::LDown(int x, int y)
{
	RefreshPlot();

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
					temp_weight -> setDst(*dst);
					Weight* weight = new Weight(*temp_weight);
					model.update_weight_set(weight);
					dst->input_weight_list.push_back(weight);
					src->output_weight_list.push_back(weight);
					dst->input_node = false;
				}
			}
			delete(temp_weight);
			state = normal;
			break;
		}
		default:
		{
		}
	}
}

void GUI::LUp(int x, int y, HWND& hWnd)
{
	RefreshPlot();

	switch(state)
	{
		case node_add_button_LDown:
		{
			if(node_add_button.isIn(x,y)) {
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
				RECT temp;
				GetWindowRect(hWnd, &temp);
				SendMessage(plotWindowHwnd, WM_CLOSE, NULL, NULL);
				plotWindowHwnd = CreateWindow(
							L"Plot",
							L"Plot",
							WS_BORDER | WS_CHILD | WS_POPUPWINDOW | WS_OVERLAPPEDWINDOW,
							temp.left - 300,
							temp.top,
							300,
							300,
							hWnd,
							(HMENU)0,
							*g_hInst,
							NULL
							);
				ShowWindow(plotWindowHwnd, SW_SHOW);
				
			}
			state = normal;
			break;
		}
		case input_button_LDown:
		{
			if(input_button.isIn(x,y)) {
				RECT temp;
				GetWindowRect(hWnd, &temp);
				SendMessage(inputWindowHwnd, WM_CLOSE, NULL, NULL);
				inputWindowHwnd = CreateWindow(
							L"Input",
							L"Input Data",
							WS_BORDER | WS_CHILD | WS_POPUPWINDOW | WS_OVERLAPPEDWINDOW,
							temp.left - 300,
							temp.top + 300,
							300,
							300,
							hWnd,
							(HMENU)0,
							*g_hInst,
							NULL
							);
				ShowWindow(inputWindowHwnd, SW_SHOW);
					
				int cnt = model.count_input_node();
				if(cnt==2)
				{
					RECT temp;
					GetWindowRect(hWnd, &temp);
					SendMessage(inputGUIWindowHwnd, WM_CLOSE, NULL, NULL);
					inputGUIWindowHwnd = CreateWindow(
								L"InputGUI",
								L"Input 2D Data",
								WS_BORDER | WS_CHILD | WS_POPUPWINDOW | WS_OVERLAPPEDWINDOW,
								temp.left - 600,
								temp.top + 300,
								300,
								300,
								hWnd,
								(HMENU)0,
								*g_hInst,
								NULL
								);
					ShowWindow(inputGUIWindowHwnd, SW_SHOW);
				}
			}
			state = normal;
			break;
		}
		case output_button_LDown:
		{
			if(output_button.isIn(x,y)) {
				RECT temp;
				GetWindowRect(hWnd, &temp);
				SendMessage(outputWindowHwnd, WM_CLOSE, NULL, NULL);
				outputWindowHwnd = CreateWindow(
							L"Output",
							L"Output",
							WS_BORDER | WS_CHILD | WS_POPUPWINDOW | WS_OVERLAPPEDWINDOW,
							temp.right,
							temp.top,
							300,
							600,
							hWnd,
							(HMENU)0,
							*g_hInst,
							NULL
							);
				ShowWindow(outputWindowHwnd, SW_SHOW);
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
	RefreshPlot();

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
	RefreshPlot();

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

bool is_node_in_box(LPRECT select_box, Node* node)
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

bool plot_verify(Node* idx, bool reset)
{
	static set<Node*> visit;
	if(reset)
	{
		visit = set<Node*>();
	}

	if(visit.count(idx) != 0)
		return true;

	visit.insert(idx);
	idx->plot_mode = true;

	if(idx->plot_input) {
		return true;
	}
	
	if(idx->input_weight_list.size()==0) {
		return false;
	}

	bool ret = true;
	for(auto w : idx->input_weight_list) {
		w->plot_mode = true;
		bool temp = plot_verify(w->getSrc(), false);
		ret = ret && temp;
	}
	return ret;
}

void GUI::end_box_drag()
{
	if(state==left_box_drag)
	{
		for(auto node = model.get_first_node_iter(); node != model.get_last_node_iter(); node++)
		{
			if(is_node_in_box(&select_box_left, *node))
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
			if(is_node_in_box(&select_box_right, *node))
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
					Weight* weight = new Weight(left, right);
					model.update_weight_set(weight);
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
	RefreshPlot();

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
	RefreshPlot();

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
				Node* node = new Node(original_node->getXY().x, original_node->getXY().y, original_node->get_idx());
				node -> input_weight_list = original_node -> input_weight_list;
				node -> output_weight_list = original_node -> output_weight_list;
				copied_list.push_back(make_pair(node, copied_node));
			}
		}
		for(auto p : copied_list) {
			for(auto w : p.first -> input_weight_list) {
				Node* src = w -> getSrc();
				Weight* weight = new Weight(src, p.second);
				src -> output_weight_list.push_back(weight);
				p.second -> input_weight_list.push_back(weight);
				model.update_weight_set(weight);
			}
			for(auto w : p.first -> output_weight_list) {
				Node* dst = w -> getDst();
				Weight* weight = new Weight(p.second, dst);
				dst -> input_weight_list.push_back(weight);
				p.second -> output_weight_list.push_back(weight);
				model.update_weight_set(weight);
			}
		}
		if(!shift_down){
			for(int i=0; i<(int)copied_list.size(); i++) {
				delete(copied_list[i].first);
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
	RefreshPlot();

	if(input.compare("delete")==0) {
		if(is_any_selected_left){
			for(auto pnode = select_list_left.begin(); pnode != select_list_left.end(); pnode++) {
				Node* node = *pnode;
				for(int j=0; j<(int)node -> input_weight_list.size(); j++) {
					Weight* weight = node -> input_weight_list[j];
					Node* src = weight -> getSrc();
					for(int k=0; k<(int)src->output_weight_list.size(); k++) {
						if(src->output_weight_list[k]->getDst() == node) {
							model.remove_weight_set(src->output_weight_list[k]);
							src->output_weight_list.erase(src->output_weight_list.begin() + k);
							k--;
						}
					}
				}
				for(int j=0; j<(int)node -> output_weight_list.size(); j++) {
					Weight* weight = node -> output_weight_list[j];
					Node* dst = weight -> getDst();
					for(int k=0; k<(int)dst->input_weight_list.size(); k++) {
						if(dst->input_weight_list[k]->getSrc() == node) {
							model.remove_weight_set(dst->input_weight_list[k]);
							dst->input_weight_list.erase(dst->input_weight_list.begin() + k);
							k--;
						}
					}
				}
				if(node -> plot_input) {
					plot_in_list.erase(node);
					plot_input_count--;
				}
				if(node -> plot_border_output || node -> plot_output) {
					plot_out_list.erase(node);
				}
				model.erase_node(node);
			}
			select_list_left.clear();
			model.reindex();
		}
	} else if(input.compare("shift")==0) {
		shift_down = false;
	}
}

COLORREF color_add(COLORREF ac, COLORREF bc)
{
	int r = GetRValue(ac) + GetRValue(bc);
	int g = GetGValue(ac) + GetGValue(bc);
	int b = GetBValue(ac) + GetBValue(bc);
	if(r>255) r = 255;
	if(g>255) g = 255;
	if(b>255) b = 255;
	return RGB(r,g,b);
}

COLORREF color_scalar_mul(long double k, COLORREF ac)
{
	int r = GetRValue(ac) * k;
	int g = GetGValue(ac) * k;
	int b = GetBValue(ac) * k;
	if(r>255) r = 255;
	if(g>255) g = 255;
	if(b>255) b = 255;
	return RGB(r,g,b);
}

void GUI::RefreshPlot()
{
	if(plotWindowHwnd!=NULL)
	{
		SendMessage(plotWindowHwnd, WM_TIMER, NULL, NULL);
	}
}

LRESULT CALLBACK GUI::WndProcPlot(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;

	HBITMAP hBit, OldBit;
	RECT crt;

	HPEN hPen, oldPen;

	static long double zoom = 10;

	switch(iMsg)
	{
		case WM_CREATE:
		{
			//SetTimer(hWnd, 1, 200, 0);
			break;
		}

		case WM_TIMER:
		{
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			short temp = HIWORD(wParam);
			zoom *= (1-(long double)temp/5000);
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &crt);

			MemDC = CreateCompatibleDC(hdc);
			hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
			OldBit = (HBITMAP)SelectObject(MemDC, hBit);
			//hBrush = CreateSolidBrush(RGB(255, 255, 255));
			//oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
			//hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
			//oldPen = (HPEN)SelectObject(MemDC, hPen);

			//FillRect(MemDC, &crt, hBrush);
			SetBkColor(MemDC, RGB(255, 255, 255));

			Node *input[2];
			input[0] = *(plot_in_list.begin());
			input[1] = *(plot_in_list.rbegin());

			vector<COLORREF> border_clr_sample;
			for(auto it : plot_out_list) {
				if(it -> plot_border_output) {
					border_clr_sample.push_back(RGB(rand()%128+128, rand()%128+128, rand()%128+128));
				}
			}

			const int size = 240;
			for(int i=0; i<size; i++)
			{
				input[0]->set_input(zoom*(long double)i/size - zoom/2.0);
				for(int j=0; j<size; j++)
				{
					input[1]->set_input(zoom*(long double)j/size - zoom/2.0);
					vector<long double> out_pixel;
					vector<long double> border_out_pixel;
					int cnt_b = 0, cnt_o = 0;
					for(auto it : plot_out_list) {
						long double out = it->get_output();
						if(it->plot_output) {
							out_pixel.push_back(out);
							cnt_o++;
						} else {
							if(-4*(abs(out-0.5))+1 > 0) {
								border_out_pixel.push_back(-4*(abs(out-0.5))+1);
							} else {
								border_out_pixel.push_back(0);
							}
							cnt_b++;
						}
					}
					COLORREF border_clr = RGB(0,0,0);
					for(int k=0; k<cnt_b; k++) {
						border_clr = color_add(border_clr, color_scalar_mul(border_out_pixel[k], border_clr_sample[k]));
					}
					COLORREF out_clr = RGB(0,0,0);
					for(int k=0; k<cnt_o; k++) {
						out_clr  = color_add(out_clr, color_scalar_mul(out_pixel[k], RGB(255,255,255)));
					}
					
					if(GetRValue(border_clr)+GetGValue(border_clr)+GetBValue(border_clr) > 50) {
						hPen = CreatePen(PS_SOLID, 1, border_clr);
					} else {
						hPen = CreatePen(PS_SOLID, 1, out_clr);
					}
					oldPen = (HPEN)SelectObject(MemDC, hPen);
					MoveToEx(MemDC, 20+j, 250-i, NULL); 
					LineTo(MemDC, 20+j+1, 250-i+1); 
					SelectObject(MemDC, oldPen);
					DeleteObject(hPen);
				}
			}

			for(int i=0; i<(int)input_data_list.size(); i++)
			{
				input_data_list[i]->print(MemDC, zoom);
			}

			//draw axis
			hPen = CreatePen(PS_SOLID, 2, RGB(255,255,255));
			oldPen = (HPEN)SelectObject(MemDC, hPen);
			MoveToEx(MemDC, 20, 130, NULL); 
			LineTo(MemDC, 260, 130);
			MoveToEx(MemDC, 140, 250, NULL); 
			LineTo(MemDC, 140, 10);
			SelectObject(MemDC, oldPen);
			DeleteObject(hPen);


			BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
			SelectObject(MemDC, OldBit);
			DeleteDC(MemDC);
			//SelectObject(MemDC, oldPen);
			//DeleteObject(hPen);
			//SelectObject(MemDC, oldBrush);
			//DeleteObject(hBrush);
			DeleteObject(hBit);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			plotWindowHwnd = NULL;
			break;
		}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK GUI::WndProcInput(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;

	HBITMAP hBit, OldBit;
	RECT crt;

	HPEN hPen, oldPen;

	static HWND hEdit_num, hEdit_input0, hEdit_input1, hEdit_class;
	static bool scroll_flag = false;

	switch(iMsg)
	{
		case WM_CREATE:
		{
			//SetTimer(hWnd, 1, 200, 0);
			hEdit_num = CreateWindow(
							L"edit", 
							NULL, 
							WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
							10,
							10,
							30,
							250,
							hWnd,
							(HMENU)0,
							*g_hInst,
							NULL
							);

			hEdit_input0 = CreateWindow(
							L"edit", 
							NULL, 
							WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
							40,
							10,
							80,
							250,
							hWnd,
							(HMENU)1,
							*g_hInst,
							NULL
							);

			hEdit_input1 = CreateWindow(
							L"edit", 
							NULL, 
							WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
							120,
							10,
							80,
							250,
							hWnd,
							(HMENU)2,
							*g_hInst,
							NULL
							);

			hEdit_class = CreateWindow(
							L"edit", 
							NULL, 
							WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
							200,
							10,
							20,
							250,
							hWnd,
							(HMENU)3,
							*g_hInst,
							NULL
							);
			break;
		}

		case WM_TIMER:
		{
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &crt);

			MemDC = CreateCompatibleDC(hdc);
			hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
			OldBit = (HBITMAP)SelectObject(MemDC, hBit);
			//hBrush = CreateSolidBrush(RGB(255, 255, 255));
			//oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
			//hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
			//oldPen = (HPEN)SelectObject(MemDC, hPen);

			//FillRect(MemDC, &crt, hBrush);
			SetBkColor(MemDC, RGB(255, 255, 255));

			SetWindowText(hEdit_num, L"");
			SetWindowText(hEdit_input0, L"");
			SetWindowText(hEdit_input1, L"");
			SetWindowText(hEdit_class, L"");
			for(int i=0; i<(int)input_data_list.size(); i++)
			{
				long double x = input_data_list[i]->getXY().x;
				long double y = input_data_list[i]->getXY().y;
				int data_class = input_data_list[i]->get_class();

				int len = GetWindowTextLength(hEdit_num)+1;
				vector<WCHAR> str(len);
				wstring out;
				WCHAR temp[4][20]={};
				GetWindowText(hEdit_num, &str[0], len);
				out = &str[0];
				wsprintf(temp[0],L"%d\r\n",i);
				out.append(temp[0]);
				SetWindowText(hEdit_num, (LPWSTR)out.c_str());

				len = GetWindowTextLength(hEdit_input0)+1;
				str = vector<WCHAR>(len);
				GetWindowText(hEdit_input0, &str[0], len);
				out = &str[0];
				swprintf(temp[1], 20, L"%.3Lf\r\n",x);
				out.append(temp[1]);
				SetWindowText(hEdit_input0, (LPWSTR)out.c_str());

				len = GetWindowTextLength(hEdit_input1)+1;
				str = vector<WCHAR>(len);
				GetWindowText(hEdit_input1, &str[0], len);
				out = &str[0];
				swprintf(temp[2], 20, L"%.3Lf\r\n",y);
				out.append(temp[2]);
				SetWindowText(hEdit_input1, (LPWSTR)out.c_str());

				len = GetWindowTextLength(hEdit_class)+1;
				str = vector<WCHAR>(len);
				GetWindowText(hEdit_class, &str[0], len);
				out = &str[0];
				swprintf(temp[3], 20, L"%d\r\n",data_class);
				out.append(temp[3]);
				SetWindowText(hEdit_class, (LPWSTR)out.c_str());
			}


			BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
			SelectObject(MemDC, OldBit);
			DeleteDC(MemDC);
			DeleteObject(hBit);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			short temp = HIWORD(wParam);
			if(temp>0)
			{
				SendMessage(hEdit_num, EM_SCROLL, SB_LINEUP, lParam);
				SendMessage(hEdit_input0, EM_SCROLL, SB_LINEUP, lParam);
				SendMessage(hEdit_input1, EM_SCROLL, SB_LINEUP, lParam);
				SendMessage(hEdit_class, EM_SCROLL, SB_LINEUP, lParam);	
			}
			else
			{
				SendMessage(hEdit_num, EM_SCROLL, SB_LINEDOWN, lParam);
				SendMessage(hEdit_input0, EM_SCROLL, SB_LINEDOWN, lParam);
				SendMessage(hEdit_input1, EM_SCROLL, SB_LINEDOWN, lParam);
				SendMessage(hEdit_class, EM_SCROLL, SB_LINEDOWN, lParam);
			}
			break;
		}
		case WM_COMMAND:
		{
			if(scroll_flag == false)
			{
				scroll_flag = true;
				SendMessage(hEdit_num, WM_KILLFOCUS, 0, 0);
				SendMessage(hEdit_input0, WM_KILLFOCUS, 0, 0);
				SendMessage(hEdit_input1, WM_KILLFOCUS, 0, 0);
				SendMessage(hEdit_class, WM_KILLFOCUS, 0, 0);
				SetFocus(inputWindowHwnd);
				scroll_flag = false;
			}
			/*switch(LOWORD(wParam))
			{
				case 0:
				{
					switch(HIWORD(wParam))
					{
						case EN_VSCROLL:
						{
							if(scroll_flag == false)
							{
								scroll_flag = true;
								SendMessage(hEdit_input0, EM_SCROLL, SB_LINEDOWN, lParam);
								SendMessage(hEdit_input1, EM_SCROLL, SB_LINEDOWN, lParam);
								SendMessage(hEdit_class, EM_SCROLL, SB_LINEDOWN, lParam);
								scroll_flag = false;
							}
							break;
						}
					}
					break;
				}
				case 1:
				{
					switch(HIWORD(wParam))
					{
						case EN_VSCROLL:
						{
							if(scroll_flag ==false)
							{
								scroll_flag = true;
								SendMessage(hEdit_num, EM_SCROLL, SB_LINEDOWN, lParam);
								SendMessage(hEdit_input1, EM_SCROLL, SB_LINEDOWN, lParam);
								SendMessage(hEdit_class, EM_SCROLL, SB_LINEDOWN, lParam);
								scroll_flag = false;
							}
							break;
						}
					}
					break;
				}
				case 2:
				{
					switch(HIWORD(wParam))
					{
						case EN_VSCROLL:
						{
							if(scroll_flag ==false)
							{
								scroll_flag = true;
								SendMessage(hEdit_num, EM_SCROLL, wParam, lParam);
								SendMessage(hEdit_input0, EM_SCROLL, wParam, lParam);
								SendMessage(hEdit_class, EM_SCROLL, wParam, lParam);
								scroll_flag = false;
							}
							break;
						}
					}
					break;
				}
				case 3:
				{
					switch(HIWORD(wParam))
					{
						case EN_VSCROLL:
						{
							if(scroll_flag ==false)
							{
								scroll_flag = true;
								SendMessage(hEdit_num, EM_SCROLL, wParam, lParam);
								SendMessage(hEdit_input0, EM_SCROLL, wParam, lParam);
								SendMessage(hEdit_input1, EM_SCROLL, wParam, lParam);
								scroll_flag = false;
							}
							break;
						}
					}
					break;
				}
			}*/
			break;
		}
		case WM_DESTROY:
		{
			inputWindowHwnd = NULL;
			break;
		}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK GUI::WndProcInputGUI(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;

	HBITMAP hBit, OldBit;
	RECT crt;

	HPEN hPen, oldPen;

	static GUISetClassButton class0_button(15,15,10, 0, WHITE, true);
	static GUISetClassButton class1_button(40,15,10, 1, RED, false);

	static long double zoom = 10;

	static int input_class = 0;
	int xpos,ypos;
	const int size = 240;
	switch(iMsg)
	{
		case WM_CREATE:
		{
			//SetTimer(hWnd, 1, 100, 0);
			break;
		}

		case WM_TIMER:
		{
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			short temp = HIWORD(wParam);
			zoom *= (1-(long double)temp/5000);
			RefreshPlot();
			SendMessage(inputGUIWindowHwnd, WM_TIMER, NULL, NULL);
			break;
		}

		case WM_PAINT:
		{
			SendMessage(inputWindowHwnd, WM_TIMER, NULL, NULL);
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &crt);

			MemDC = CreateCompatibleDC(hdc);
			hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
			OldBit = (HBITMAP)SelectObject(MemDC, hBit);
			//hBrush = CreateSolidBrush(RGB(255, 255, 255));
			//oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
			//hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
			//oldPen = (HPEN)SelectObject(MemDC, hPen);

			//FillRect(MemDC, &crt, hBrush);
			SetBkColor(MemDC, RGB(255, 255, 255));

			class0_button.print(MemDC);
			class1_button.print(MemDC);

			for(int i=0; i<(int)input_data_list.size(); i++)
			{
				input_data_list[i]->print(MemDC, zoom);
			}


			//draw axis
			hPen = CreatePen(PS_SOLID, 2, RGB(255,255,255));
			oldPen = (HPEN)SelectObject(MemDC, hPen);
			MoveToEx(MemDC, 20, 130, NULL); 
			LineTo(MemDC, 260, 130);
			MoveToEx(MemDC, 140, 250, NULL); 
			LineTo(MemDC, 140, 10);
			SelectObject(MemDC, oldPen);
			DeleteObject(hPen);

			BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
			SelectObject(MemDC, OldBit);
			DeleteDC(MemDC);
			//SelectObject(MemDC, oldPen);
			//DeleteObject(hPen);
			//SelectObject(MemDC, oldBrush);
			//DeleteObject(hBrush);
			DeleteObject(hBit);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_CHAR:
		{
			switch(wParam)
			{
				case 26:
				{
					if(input_data_list.size()>0)
					{
						InputData* temp = input_data_list[input_data_list.size()-1];
						input_data_set[temp->get_class()].erase(temp);
						input_data_list.pop_back();
						SendMessage(inputGUIWindowHwnd, WM_TIMER, NULL, NULL);
					}
					break;
				}
			}
			break;
		}

		case WM_LBUTTONDOWN:
		{	
			xpos = GET_X_LPARAM(lParam);
			ypos = GET_Y_LPARAM(lParam);

			if(class0_button.isIn(xpos, ypos))
			{
				class0_button.selected = true;
				class1_button.selected = false;
				input_class = 0;
				SendMessage(inputGUIWindowHwnd, WM_TIMER, NULL, NULL);
				break;
			}
			if(class1_button.isIn(xpos, ypos))
			{
				class0_button.selected = false;
				class1_button.selected = true;
				input_class = 1;
				SendMessage(inputGUIWindowHwnd, WM_TIMER, NULL, NULL);
				break;
			}
			if(20 <= xpos && xpos <= 260 && 10 <= ypos && ypos <= 250)
			{
				xpos = xpos - 20;
				ypos = 250 - ypos;
				input_data_list.push_back(new InputData(zoom*(long double)xpos/size - zoom/2.0, zoom*(long double)ypos/size - zoom/2.0, input_class));
				input_data_set[input_class].insert(input_data_list[input_data_list.size()-1]);
				RefreshPlot();
				SendMessage(inputGUIWindowHwnd, WM_TIMER, NULL, NULL);
				break;
			}
			break;
		}
		case WM_DESTROY:
		{
			inputGUIWindowHwnd = NULL;
			break;
		}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK GUI::WndProcOutput(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;

	HBITMAP hBit, OldBit;
	RECT crt;

	HPEN hPen, oldPen;

	switch(iMsg)
	{
		case WM_CREATE:
		{
			//SetTimer(hWnd, 1, 200, 0);
			break;
		}

		case WM_TIMER:
		{
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &crt);

			MemDC = CreateCompatibleDC(hdc);
			hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
			OldBit = (HBITMAP)SelectObject(MemDC, hBit);
			//hBrush = CreateSolidBrush(RGB(255, 255, 255));
			//oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
			//hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
			//oldPen = (HPEN)SelectObject(MemDC, hPen);

			//FillRect(MemDC, &crt, hBrush);
			SetBkColor(MemDC, RGB(255, 255, 255));



			BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
			SelectObject(MemDC, OldBit);
			DeleteDC(MemDC);
			//SelectObject(MemDC, oldPen);
			//DeleteObject(hPen);
			//SelectObject(MemDC, oldBrush);
			//DeleteObject(hBrush);
			DeleteObject(hBit);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			outputWindowHwnd = NULL;
			break;
		}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}