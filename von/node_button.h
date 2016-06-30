#ifndef __NODE_BUTTON__
#define __NODE_BUTTON__

#include "node.h"


class NodeButton : public Node
{
private:
	int x, y,r;
	bool clicked;
public:
	NodeButton(int x, int y, int r);
	void print(HDC hdc);
	/*bool isIn(int x, int y);
	void LDown();
	bool LUp();*/
};

#endif