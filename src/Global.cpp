#include "Global.h"

intvec2 intvec2::operator+(const intvec2& v)
{
	int ResultX = this->x + v.x;
	int ResultY = this->y + v.y;
	return {ResultX, ResultY};
}

intvec2 intvec2::operator-(const intvec2& v)
{
	int ResultX = this->x - v.x;
	int ResultY = this->y - v.y;
	return { ResultX, ResultY };
}


// A mathematical mod function, since  -1 mod 4 evaluates to -1 in C++ (!?)
int MathMod(int a, int b)
{
	// Assumes b > 0
	// Works for positive or negative a.
	return (a % b + b) % b;
}
