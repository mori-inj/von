#ifndef __PRINT__
#define __PRINT__

#include <Windows.h>


#define WHITE RGB(222,222,222)
#define BRIGHTWHITE RGB(255,255,255)
#define BLACK RGB(0,0,0)
#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLUE RGB(0,0,255)
#define YELLOW RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define CYAN RGB(0,255,255)


void EllipseLine(HDC hdc, int x, int y, int r, int d, COLORREF color, COLORREF bg);

#endif