#ifndef __COORD__
#define __COORD__

template <class T>
class Coord
{
public:
	T x;
	T y;
	Coord() {};
	Coord(T x, T y) { this->x = x, this->y = y; }
};

#endif