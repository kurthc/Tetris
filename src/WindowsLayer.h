#pragma once

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <map>
#include "Global.h"
#include "Game.h"
#include "..\Tetris\resource.h"



constexpr int GAME_WINDOW_WIDTH = 800;
constexpr int GAME_WINDOW_HEIGHT = 800;
constexpr int GAME_MAP_LEFT = 100;
constexpr int GAME_MAP_TOP = 20;
constexpr int BLOCK_WIDTH = 25;
constexpr int BLOCK_HEIGHT = BLOCK_WIDTH;

enum BitmapIndex {
	BlockBlack = 0, BlockBlue = 1, BlockGreen = 2, BlockPurple = 3, BlockRed = 4, BlockYellow = 5, Count = 6
};

class bitmap_manager
{
public:
	HBITMAP Bitmap[6];
	//bitmap_manager();
	void LoadBitmaps(HINSTANCE);
};

//constexpr int BLOCK_BLACK = 0;
//constexpr int BLOCK_BLUE = 1;
//constexpr int BLOCK_GREEN = 2;
//constexpr int BLOCK_PURPLE = 3;
//constexpr int BLOCK_RED = 4;
//constexpr int BLOCK_YELLOW = 5;

static int CountsPerSecond;                // Used to convert the timer to seconds.
static HDC MemoryDeviceContext;
static HBITMAP MemoryDeviceContextBitmap;
static game_state* GlobalGameState;
static keyboard_info* KeyboardInfo;

static bitmap_manager* BitmapManager;
static HBITMAP BitmapBlockPurple;
static HBITMAP BitmapBlockRed;

struct timing_information
{
	double Seconds;
	int Counts;
	LARGE_INTEGER LICounts;
};



//static key_mapping KeyMapping;

bool Win32SetUpMemoryDeviceContext(HDC DeviceContext);
static void Win32AddConsole();
timing_information GetSeconds();
static void Win32DrawClientArea(HDC DeviceContext);
static void Win32DrawGameMap();
void Win32DrawRectangle(HDC DeviceContext, int x, int y, int x2, int y2, int R, int G, int B);
static void Wind32DrawFallingPiece();
LRESULT Win32ProcessKeyboardMessage(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);