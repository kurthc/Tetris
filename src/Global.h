#pragma once

#include <Windows.h>
#include <vector>

constexpr float TargetFPS = 60.0f;

template <typename T> inline T const& MAX(T const& X, T const& Y)
{
	return X > Y ? X : Y;
}

template <typename T> inline T const& MIN(T const& X, T const& Y)
{
	return X > Y ? Y : X;
}

class intvec2
{
public:
	int x;
	int y;
	intvec2() : x(0), y(0) {}
	intvec2(int x, int y) : x(x), y(y) {}

	intvec2 operator+(const intvec2& v);
};

class key_state
{
public:
	WPARAM VKey;
	bool IsDown = false;
	bool WasDown = false;    //Last frame

	key_state() : VKey(0) {};
	key_state(WPARAM wParam) : VKey(wParam) {};
};

class keyboard_info
{
public:
	std::vector<key_state> Key;
	keyboard_info();
	float RepeatTimer;
	int size() { return Key.size(); }
};


