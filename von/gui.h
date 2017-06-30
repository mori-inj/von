#ifndef __GUI__
#define __GUI__

#include "node_button.h"
#include "plot_button.h"
#include "input_button.h"
#include "output_button.h"
#include "model.h"
#include "temp_weight.h"
#include "input_data.h"

#define MENU_NUM 4
#define IDM_PLOT_INPUT 100
#define IDM_PLOT_OUTPUT 101
#define IDM_PLOT_BORDER_OUTPUT 102
#define IDM_RESET 103

const int WIDTH = 800;
const int HEIGHT = 600;
const int MARGIN = 7;

using namespace std;


template<class it1, class it2>
bool intersect(it1 first1, it1 last1, it2 first2, it2 last2)
{
	while (first1 != last1 && first2 != last2)
	{
		if(*first1 < *first2)
		{
			++first1;
			continue;
		}
		if(*first2 < *first1)
		{
			++first2;
			continue;
		}
		return true;
	}
	return false;
}


class GUI
{
private:
	enum State { 
		normal, 
		node_add_button_LDown, plot_button_LDown, input_button_LDown, output_button_LDown,
		node_add_mouse_move,
		node_LDown, node_drag,
		weight_add_mouse_move,
		left_box_drag, right_box_drag
	};
	State state;
	NodeButton node_add_button;
	PlotButton plot_button;
	InputButton input_button;
	OutputButton output_button;
	Model model;
	Node* temp_node;
	TempWeight* temp_weight;
	int node_idx;
	
	set<Node*> select_list_left;
	set<Node*> select_list_right;
	bool is_any_selected_left, is_any_selected_right;
	RECT select_box_left, select_box_right;

	int plot_input_count;
	static set<Node*> plot_in_list;
	static set<Node*> plot_out_list;

	vector<Node*> copy_list;
	bool shift_down;

	static HWND plotWindowHwnd, inputWindowHwnd, inputGUIWindowHwnd, outputWindowHwnd;
	static HINSTANCE* g_hInst;
	static vector<InputData*> input_data_list;
	static set<InputData*> input_data_set[2];

public:
	GUI(HINSTANCE* g_hInst);
	void print(HDC MemDC, HWND& hWnd);
	void LDown(int x, int y);
	void LUp(int x, int y, HWND& hWnd);
	void RDown(int x, int y, HWND* hWnd);
	void RUp(int x, int y);
	void mouse_move(int x, int y);
	void end_box_drag();
	void command(int msg);
	void keyboard_down(string input);
	void keyboard_up(string input);


	static void RefreshPlot();
	static LRESULT CALLBACK WndProcPlot(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProcInput(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProcInputGUI(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProcOutput(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

#endif
