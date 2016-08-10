#include "function.h"


long double identity(long double x)
{
	return x;
}
long double linear(long double a, long double x)
{
	return a*x;
}

long double step(long double x)
{
	if(x<0)
		return 0;
	else
		return 1;
}

long double sigmoid(long double x)
{
	return 1/(1+powl(e,-x));
}

long double sigmoid(long double b, long double x)
{
	return 1/(1+powl(e,-b*x));
}