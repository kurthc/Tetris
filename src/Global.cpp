#include "Global.h"

intvec2 intvec2::operator+(const intvec2& v)
{
	int ResultX = this->x + v.x;
	int ResultY = this->y + v.y;
	return {ResultX, ResultY};
}