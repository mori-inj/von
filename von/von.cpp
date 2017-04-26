#include <Windows.h>
#include <gdiplus.h>
#include <Windowsx.h>
#include <time.h>
#include <vector>
#include <set>
#include <iostream>
//#include "resource.h"
#include "print.h"
#include "node_button.h"
#include "plot_button.h"
#include "node.h"
#include "weight.h"
#include "temp_weight.h"

using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus")
#pragma warning(disable:4018)

#define MENU_NUM 4
#define IDM_PLOT_INPUT 100
#define IDM_PLOT_OUTPUT 101
#define IDM_PLOT_BORDER_OUTPUT 102
#define IDM_RESET 103


typedef int NodeIdx;
static vector<Node*> node_list;

static set<Node*> plot_in_list;
static set<Node*> plot_out_list;
static int plot_input_count = 0;


const int WIDTH = 800;
const int HEIGHT = 600;
const int MARGIN = 7;

HINSTANCE g_hInst;
HWND hWndMain;
HWND plotWindowHwnd;
LPCTSTR lpszClass = TEXT("GdiPlusStart");

void OnPaint(HDC hdc, int ID, int x, int y);
void OnPaintA(HDC hdc, int ID, int x, int y, double alpha);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcPlot(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int whichNodeClicked(int xpos, int ypos);
bool plot_verify(Node* idx, bool reset);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND     hWnd;
	MSG		 msg;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;
	if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok)
	{
		MessageBox(NULL, TEXT("GDI+ 라이브러리를 초기화할 수 없습니다."), TEXT("알림"), MB_OK);
		return 0;
	}


	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"VoN";
	RegisterClass(&WndClass);

	//
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)WndProcPlot;
	WndClass.lpszClassName = L"Plot";
	RegisterClass(&WndClass);
	//

	hWnd = CreateWindow(
		L"VoN",
		L"VoN",
		WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXFULLSCREEN) / 2 - WIDTH/2,
		GetSystemMetrics(SM_CYFULLSCREEN) / 2 - HEIGHT/2,
		WIDTH,
		HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;

	HBITMAP hBit, OldBit;
	RECT crt;

	//HPEN hPen, oldPen;
	static TempWeight temp_weight;
	static NodeButton node_add_button(35,35,25);
	static PlotButton plot_button(100,35,25);

	static bool node_add_flag;
	static bool node_move_flag;
	static bool node_moved;
	static bool weight_add_flag;
	static NodeIdx node_idx;
	static bool mouse_move_end = false;

	int xpos,ypos;

	switch (iMsg)
	{
		case WM_CREATE:
		{
			SetTimer(hWnd, 1, 10, 0);
			srand((unsigned)time(NULL));
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

			node_add_button.print(MemDC);
			plot_button.print(MemDC);
			if(weight_add_flag)
			{
				temp_weight.print(MemDC);
			}

			for(int i=node_list.size()-1; i>=0; i--)
			{
				node_list[i] -> printWeight(MemDC);
			}

			for(int i=node_list.size()-1; i>=0; i--)
			{
				node_list[i] -> print(MemDC);
			}



			for (int i = 0; i<(int)node_list.size(); i++)
			{
				node_list[i]->plot_mode = false;
				for (int j = 0; j<(int)node_list[i]->weight_list.size(); j++)
					node_list[i]->weight_list[j]->plot_mode = false;
			}
			bool ret = true;
			for(auto it : plot_out_list)
			{
				bool temp  = plot_verify(it, true);
				ret = ret && temp;
			}
			if(ret && plot_input_count == 2 && plot_out_list.size()>0)
				plot_button.plot_mode = true;
			else
				plot_button.plot_mode = false;


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


		case WM_LBUTTONDOWN:
		{
			xpos = GET_X_LPARAM(lParam);
			ypos = GET_Y_LPARAM(lParam);

			//node added and fix
			//end of mouse_move
			if(node_add_flag && !weight_add_flag)
			{
				node_add_flag = false;
				mouse_move_end = true;
				break;
			}

			//node add button clicked(down)
			//prepare for mouse_move
			if(node_add_button.isIn(xpos,ypos) && !weight_add_flag)
			{
				node_add_button.LDown();
				break;
			}

			if(plot_button.isIn(xpos, ypos) && !weight_add_flag && !node_add_flag && plot_button.plot_mode)
			{
				plot_button.LDown();
				break;
			}

			//weight added and fix
			//end of mouse_move
			if(weight_add_flag && !node_add_flag)
			{
				//find dest
				int i = whichNodeClicked(xpos, ypos);
				if(i != -1 && i!=node_idx)
				{
					temp_weight.setDst(*node_list[i]);
					node_list[i]->weight_list.push_back(new Weight(temp_weight));
					node_list[i]->input_node = false;
				}
				weight_add_flag = false;
				mouse_move_end = true;
				break;
			}



			//node clicked(down): either drag(move) or add weight(toggle)
			//prepare for mouse_move || start of drag
			int i = whichNodeClicked(xpos, ypos);
			if(i != -1)
			{
				SetCapture(hWnd);
				node_move_flag = true;
				node_list[i]->LDown();
				node_idx = i;
			}
			 

			break;
		}


		case WM_LBUTTONUP:
		{
			if(!mouse_move_end)
			{
				xpos = GET_X_LPARAM(lParam);
				ypos = GET_Y_LPARAM(lParam);

				//node add button clicked(down)
				//start of mouse_move
				if(node_add_button.isIn(xpos,ypos) && !node_move_flag)
				{
					node_add_flag = node_add_button.LUp();
					if(node_add_flag)
						node_list.push_back(new Node(xpos,ypos));
					node_add_button.LUp();
					break;
				}
				node_add_button.LUp();

				if(plot_button.isIn(xpos,ypos) && plot_button.plot_mode)
				{
					if(plot_button.LUp())
					{
						
						RECT temp;
						GetWindowRect(hWnd, &temp);
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
									g_hInst,
									NULL
									);
						ShowWindow(plotWindowHwnd, SW_SHOW);
					}
				}
				plot_button.LUp();


				//add wieght
				//start of mouse_move
				if(!node_moved)
				{
					ReleaseCapture();
					node_move_flag = false;
					int i = whichNodeClicked(xpos, ypos);
					if(i!=-1)
					{
						if(node_list[i]->LUp())
						{
							weight_add_flag = true;
							temp_weight = TempWeight(node_list[i]);
							temp_weight.setDXY(xpos,ypos);
						}
					}
					break;
				}

				//end of drag
				if(node_move_flag)
				{
					ReleaseCapture();
					node_moved = false;
					node_move_flag = false;
				}

				for(int i=0; i<node_list.size(); i++)
				{
					node_list[i]->LUp();
				}
			}
			else
			{
				mouse_move_end = false;
			}
			break;
		}

		case WM_RBUTTONDOWN:
		{
			if(!mouse_move_end && !node_add_flag && !node_move_flag && !weight_add_flag)
			{
				xpos = GET_X_LPARAM(lParam);
				ypos = GET_Y_LPARAM(lParam);

				int i = whichNodeClicked(xpos, ypos);
				if(i!=-1)
				{
					node_list[i]->RDown();
					node_idx = i;

					HMENU hPopupMenu = CreatePopupMenu();
					Rect rect;
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_INPUT+MENU_NUM*i, (LPCWSTR)L"Plot as input");
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_OUTPUT+MENU_NUM*i, (LPCWSTR)L"Plot as output");
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_BORDER_OUTPUT+MENU_NUM*i, (LPCWSTR)L"Plot as border output");
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_RESET+MENU_NUM*i, (LPCWSTR)L"Reset");
					if(GetWindowRect(hWnd,(LPRECT)&rect))
						TrackPopupMenu(hPopupMenu, 
							TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, 
							xpos+rect.GetLeft()+8, ypos+rect.GetTop()+29, 0, 
							hWnd, NULL);
				}

			}
			break;
		}


		case WM_RBUTTONUP:
		{
			xpos = GET_X_LPARAM(lParam);
			ypos = GET_Y_LPARAM(lParam);
			for(int i=0; i<node_list.size(); i++)
			{
				node_list[i]->RUp();
			}
			break;
		}


		case WM_MOUSEMOVE:
		{
			xpos = GET_X_LPARAM(lParam);
			ypos = GET_Y_LPARAM(lParam);

			//node를 드래그로 이동 중에 화면 밖으로 나간 경우
			if( xpos<=0+MARGIN || ypos<=0+MARGIN || xpos>=WIDTH-1-MARGIN || ypos>=HEIGHT-1-MARGIN )
			{
				ReleaseCapture();
				node_move_flag = false;
			}

			//node를 추가하기 위해 마우스를 움직이고 있는 경우, 가장 마지막에 추가된 node의 위치를 변경
			if(node_add_flag)
			{
				node_list[node_list.size()-1]->setXY(xpos,ypos);
			}

			//node를 드래그로 이동 중인 경우
			if(node_move_flag)
			{
				//node자체의 위치를 옮긺
				node_list[node_idx]->setXY(xpos,ypos);
				node_moved = true;
			}

			//weight를 추가 중인 경우
			if(weight_add_flag)
			{
				temp_weight.setDXY(xpos,ypos);
			}
			break;
		}

		case WM_COMMAND:
		{
			int msg = LOWORD(wParam);
			if (msg >= IDM_PLOT_INPUT && msg%MENU_NUM==0)
			{
				if (plot_input_count < 2)
				{
					int i = (msg - IDM_PLOT_INPUT)/MENU_NUM;
					if(node_list[i]->plot_output || node_list[i]->plot_border_output)
					{
						plot_out_list.erase(node_list[i]);
					}

					node_list[i]->plot_input = true;
					node_list[i]->plot_border_output = false;
					node_list[i]->plot_output = false;
					plot_input_count++;
					plot_in_list.insert(node_list[i]);
				}
			}
			else if (msg >= IDM_PLOT_OUTPUT && msg%MENU_NUM==1)
			{
				int i = (msg - IDM_PLOT_OUTPUT)/MENU_NUM;
				if(node_list[i]->plot_input)
				{
					plot_input_count--;
					plot_in_list.erase(node_list[i]);
				}
				if(node_list[i]->plot_border_output)
				{
					plot_out_list.erase(node_list[i]);
				}

				node_list[i]->plot_input = false;
				node_list[i]->plot_border_output = false;
				node_list[i]->plot_output = true;

				plot_out_list.insert(node_list[i]);

				
			}
			else if (msg >= IDM_PLOT_BORDER_OUTPUT && msg%MENU_NUM==2)
			{
				int i = (msg - IDM_PLOT_OUTPUT)/MENU_NUM;
				if(node_list[i]->plot_input)
				{
					plot_input_count--;
					plot_in_list.erase(node_list[i]);
				}
				if(node_list[i]->plot_output)
				{
					plot_out_list.erase(node_list[i]);
				}

				node_list[i]->plot_input = false;
				node_list[i]->plot_output = false;
				node_list[i]->plot_border_output = true;

				plot_out_list.insert(node_list[i]);
			}
			else if (msg >= IDM_RESET && msg%MENU_NUM==3)
			{
				int i = (msg - IDM_RESET)/MENU_NUM;
				if(node_list[i]->plot_input)
				{
					plot_input_count--;
					plot_in_list.erase(node_list[i]);
				}
				if(node_list[i]->plot_output || node_list[i]->plot_border_output)
				{
					plot_out_list.erase(node_list[i]);
				}

				node_list[i]->plot_input = false;
				node_list[i]->plot_output = false;
				node_list[i]->plot_border_output = false;
			}
			break;
		}


		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK WndProcPlot(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
			SetTimer(hWnd, 1, 200, 0);
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

			const int size = 240;
			for(int i=0; i<size; i++)
			{
				input[0]->set_input(zoom*(long double)i/size - zoom/2);
				for(int j=0; j<size; j++)
				{
					input[1]->set_input(zoom*(long double)j/size - zoom/2);
					vector<long double> out_pixel;
					vector<long double> border_out_pixel(3,0);
					int cnt = 0;
					for(auto it : plot_out_list)
					{
						long double out = it->get_output();
						if(it->plot_output)
							out_pixel.push_back(out);
						else
						{
							if(-4*(abs(out-0.5))+1 > 0)
								border_out_pixel[cnt++] = -4*(abs(out-0.5))+1;
							else
								border_out_pixel[cnt++] = 0;
						}
					}
					COLORREF border_clr = RGB(border_out_pixel[0]*255, border_out_pixel[1]*255, border_out_pixel[2]*255);
					COLORREF out_clr = RGB(out_pixel[0]*255, out_pixel[0]*255, out_pixel[0]*255);
					
					if(GetRValue(border_clr)+GetGValue(border_clr)+GetBValue(border_clr) > 50)
						hPen = CreatePen(PS_SOLID, 1, border_clr);
					else
						hPen = CreatePen(PS_SOLID, 1, out_clr);
					oldPen = (HPEN)SelectObject(MemDC, hPen);
					MoveToEx(MemDC, 20+j, 250-i, NULL); 
					LineTo(MemDC, 20+j+1, 250-i+1); 
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
			}


			//draw axis
			hPen = CreatePen(PS_SOLID, 2, RGB(255,255,255));
			oldPen = (HPEN)SelectObject(MemDC, hPen);
			MoveToEx(MemDC, 20, 130, NULL); 
			LineTo(MemDC, 260, 130);
			MoveToEx(MemDC, 140, 250, NULL); 
			LineTo(MemDC, 140, 10);
			SelectObject(hdc, oldPen);
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

int whichNodeClicked(int xpos, int ypos)
{
	for(int i=0; i<node_list.size(); i++)
	{
		if(node_list[i]->isIn(xpos,ypos))
		{
			return i;
		}
	}
	return -1;
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

	if(idx->plot_input)
		return true;
	
	if(idx->weight_list.size()==0)
		return false;

	bool ret = true;
	for(int i=0; i<(int)idx->weight_list.size(); i++)
	{
		idx->weight_list[i]->plot_mode = true;
		bool temp = plot_verify(idx->weight_list[i]->getSrc(), false);
		ret = ret && temp;
	}
	return ret;
}

void OnPaint(HDC hdc, int ID, int x, int y)
{
	Graphics G(hdc);
	HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(ID), TEXT("PNG"));
	if (!hResource)
		return;

	DWORD imageSize = SizeofResource(g_hInst, hResource);
	HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	LPVOID pData = LockResource(hGlobal);

	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	IStream *pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image I(pStream);
	pStream->Release();
	if (I.GetLastStatus() != Ok) return;

	G.DrawImage(&I, x, y, I.GetWidth(), I.GetHeight());
}

void OnPaintA(HDC hdc, int ID, int x, int y, double alpha)
{
	Graphics G(hdc);
	HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(ID), TEXT("PNG"));
	if (!hResource)
		return;

	ColorMatrix ClrMatrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, (Gdiplus::REAL)alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes ImgAttr;
	ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	DWORD imageSize = SizeofResource(g_hInst, hResource);
	HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	LPVOID pData = LockResource(hGlobal);

	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	IStream *pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image I(pStream);
	pStream->Release();
	if (I.GetLastStatus() != Ok) return;

	RectF destination(0, 0, (Gdiplus::REAL)I.GetWidth(), (Gdiplus::REAL)I.GetHeight());
	G.DrawImage(&I, destination, (Gdiplus::REAL)x, (Gdiplus::REAL)y, (Gdiplus::REAL)I.GetWidth(), (Gdiplus::REAL)I.GetHeight(), UnitPixel, &ImgAttr);
}

