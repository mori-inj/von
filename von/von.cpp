#include <Windows.h>
#include <gdiplus.h>
#include <Windowsx.h>
#include <vector>
#include <iostream>
//#include "resource.h"
#include "print.h"
#include "node_button.h"
#include "node.h"

using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus")


typedef int NodeIdx;



const int WIDTH = 800;
const int HEIGHT = 600;
const int MARGIN = 7;

HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiPlusStart");

void OnPaint(HDC hdc, int ID, int x, int y);
void OnPaintA(HDC hdc, int ID, int x, int y, double alpha);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

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
	static NodeButton node_button(35,35,25);
	static vector<Node*> node_list;

	static bool node_add_flag;
	static bool node_move_flag;
	static NodeIdx node_idx;

	int xpos,ypos;

	switch (iMsg)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, 0);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
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
		
		node_button.print(MemDC);
		for(int i=0; i<node_list.size(); i++)
			node_list[i]->print(MemDC);


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

	case WM_LBUTTONDOWN:
		xpos = GET_X_LPARAM(lParam);
		ypos = GET_Y_LPARAM(lParam);
		if(node_add_flag)
		{
			node_add_flag = false;
		}
		if(node_button.isIn(xpos,ypos))
			node_button.LDown();
		break;
	case WM_LBUTTONUP:
		xpos = GET_X_LPARAM(lParam);
		ypos = GET_Y_LPARAM(lParam);
		if(node_button.isIn(xpos,ypos))
		{
			node_add_flag = node_button.LUp();
			if(node_add_flag)
				node_list.push_back(new Node(xpos,ypos));
		}
		node_button.LUp();
		break;

	case WM_RBUTTONDOWN:
		xpos = GET_X_LPARAM(lParam);
		ypos = GET_Y_LPARAM(lParam);
		for(int i=0; i<node_list.size(); i++)
		{
			if(node_list[i]->isIn(xpos,ypos))
			{
				SetCapture(hWnd);
				node_move_flag = true;
				node_list[i]->RDown();
				node_idx = i;
				break;
			}
		}
		break;

	case WM_RBUTTONUP:
		xpos = GET_X_LPARAM(lParam);
		ypos = GET_Y_LPARAM(lParam);
		if(node_move_flag)
		{
			ReleaseCapture();
			node_move_flag = false;
		}
		for(int i=0; i<node_list.size(); i++)
			node_list[i]->RUp();
		break;

	case WM_MOUSEMOVE:
		xpos = GET_X_LPARAM(lParam);
		ypos = GET_Y_LPARAM(lParam);
		cout << "x: " << xpos << "  y: "<< ypos << endl;
		if( xpos<=0+MARGIN || ypos<=0+MARGIN || xpos>=WIDTH-1-MARGIN || ypos>=HEIGHT-1-MARGIN )
		{
			ReleaseCapture();
			cout << "leave!!" << endl;
			node_move_flag = false;
		}
		if(node_add_flag)
		{
			node_list[node_list.size()-1]->setXY(xpos,ypos);
		}
		if(node_move_flag)
		{
			node_list[node_idx]->setXY(xpos,ypos);
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
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
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
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

	RectF destination(0, 0, I.GetWidth(), I.GetHeight());
	G.DrawImage(&I, destination, x, y, I.GetWidth(), I.GetHeight(), UnitPixel, &ImgAttr);
}