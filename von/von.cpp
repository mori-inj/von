#include <Windows.h>
#include <gdiplus.h>
#include <Windowsx.h>
#include <vector>
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

#define IDM_PLOT_INPUT 100
#define IDM_PLOT_OUTPUT 101


typedef int NodeIdx;
static vector<Node*> node_list;


const int WIDTH = 800;
const int HEIGHT = 600;
const int MARGIN = 7;

HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiPlusStart");

void OnPaint(HDC hdc, int ID, int x, int y);
void OnPaintA(HDC hdc, int ID, int x, int y, double alpha);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int whichNodeClicked(int xpos, int ypos);

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
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_INPUT, (LPCWSTR)L"Plot as input");
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, IDM_PLOT_OUTPUT, (LPCWSTR)L"Plot as output");
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
				node_list[i]->RUp();
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
			case IDM_PLOT_INPUT:
				break;
			case IDM_PLOT_OUTPUT:
				break;

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
	G.DrawImage(&I, destination, x, y, (Gdiplus::REAL)I.GetWidth(), (Gdiplus::REAL)I.GetHeight(), UnitPixel, &ImgAttr);
}

