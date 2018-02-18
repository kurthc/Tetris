#pragma once

#include <Windows.h>
#include <vector>

/*
TODO:
Left/Right movement is too slow when the pieces are falling fast
Change the colors to a proper color enum and not a bitmap index.
Checking of game board bounds in game_board.
Get rid of globals.
Separate out Game.cpp
Show FPS, Score, Lines, etc. on screen
*/

constexpr float TargetFPS = 60.0f;

template <typename T> inline T const& MAX(T const& X, T const& Y)
{
	return X > Y ? X : Y;
}

template <typename T> inline T const& MIN(T const& X, T const& Y)
{
	return X > Y ? Y : X;
}

int MathMod(int a, int b);

enum BitmapIndex {
	BlockBlack = 0, BlockBlue = 1, BlockGreen = 2, BlockPurple = 3, BlockRed = 4, BlockYellow = 5,
	BlockCyan = 6, BlockWhite = 7, Count = 8
};

class intvec2
{
public:
	int x;
	int y;
	intvec2() : x(0), y(0) {}
	intvec2(int x, int y) : x(x), y(y) {}
	intvec2(int* a) : x(a[0]), y(a[1]) {}

	intvec2 operator+(const intvec2& v);
	intvec2 operator-(const intvec2& v);
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
	int IndexRight = 0;
	int IndexLeft = 0;
	int IndexDown = 0;
	int IndexTurnRight = 0;
	int IndexTurnLeft = 0;
	int IndexDrop = 0;
	int IndexNextPiece = 0;
	int IndexDebug = 0;

	enum KeyFunction { Unmapped, Right, Left, Down, TurnRight, TurnLeft, Debug, NextPiece, Drop };

	keyboard_info();
	float RepeatTimer;
	size_t size() { return Key.size(); }
	key_state& KeyRight() { return this->Key[this->IndexRight]; }
	key_state& KeyLeft() { return this->Key[this->IndexLeft]; }
	key_state& KeyDown() { return this->Key[this->IndexDown]; }
	key_state& KeyTurnRight() { return this->Key[this->IndexTurnRight]; }
	key_state& KeyTurnLeft() { return this->Key[this->IndexTurnLeft]; }
	key_state& KeyDrop() { return this->Key[this->IndexDrop]; }
	key_state& KeyNextPiece() { return this->Key[this->IndexNextPiece]; }
	key_state& KeyDebug() { return this->Key[this->IndexDebug]; }
};


