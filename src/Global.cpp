#include "Global.h"

intvec2 intvec2::operator+(const intvec2& v)
{
	int ResultX = this->x + v.x;
	int ResultY = this->y + v.y;
	return {ResultX, ResultY};
}

int ProperMod(int a, int b)
{
	//Assumes b > 0
	// Works for a positive or negative.
	return (a % b + b) % b;
}
