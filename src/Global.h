#pragma once

class intvec2
{
public:
	int x;
	int y;
	intvec2() : x(0), y(0) {}
	intvec2(int x, int y) : x(x), y(y) {}

	intvec2 operator+(const intvec2& v);
};