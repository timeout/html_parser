#include "html_parser_arith.h"

static int trunc_to_zero(int x, int y)
{
	int z;

	if (((x < 0) != (y < 0)) && (y < 0)) {
		int t = y;
		y = x;
		x = t;
	}

	z = x / y;

	return (z * y > x);
}

extern int Arith_max(int x, int y)
{
	return (x > y ? x : y);
}

extern int Arith_min(int x, int y)
{
	return (x < y ? x : y);
}

extern int Arith_div(int x, int y)
{
	int z;

	z = x / y;
	if ((trunc_to_zero(x, y)) && ((x < 0) != (y < 0)) && (x % y != 0)) {
		return (x / y - 1);
	} else {
		return (x / y);	
	}
}

extern int Arith_mod(int x, int y)
{
	if ((trunc_to_zero(x, y)) && ((x < 0) != (y < 0)) && (x % y != 0)) {
		return (x % y + y);
	} else {
		return (x % y);
	}
}

extern int Arith_ceiling(int x, int y)
{
	return (Arith_div(x, y) + (x % y != 0));
}

extern int Arith_floor(int x, int y)
{
	return (Arith_div(x,y));
}
