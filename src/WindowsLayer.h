#pragma once

#include <Windows.h>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "Global.h"
#include "Game.h"
#include "..\Tetris\resource.h"

constexpr int GAME_WINDOW_WIDTH = 800;
constexpr int GAME_WINDOW_HEIGHT = 800;
constexpr int GAME_MAP_LEFT = 200;
constexpr int GAME_MAP_TOP = 20;
constexpr int BLOCK_WIDTH = 25;
constexpr int BLOCK_HEIGHT = BLOCK_WIDTH;

const RECT ScoreRegion{ 10, 200, 100, 250 };
const RECT StatsRegion{ 10, 600, 100, 650 };
const RECT NextPieceRegion{ 500, 100, 700, 200 };

class bitmap_manager
{
public:
	HBITMAP Bitmap[BitmapIndex::Count];
	void LoadBitmaps(HINSTANCE);
};

class buffer
{
public:
	HDC MemoryDeviceContext;
	HBITMAP MemoryDeviceContextBitmap;
	game_state* GameState;   // Get rid of this later.
	bitmap_manager* BitmapManager;

	buffer(game_state*, bitmap_manager*);
	bool SetUpMemoryDeviceContext(HDC DeviceContext);
	intvec2 MapToDisplayCoordinates(intvec2 MapPosition);
	void DrawBitmap(int x, int y, int width, int height, HBITMAP Bitmap);
	void DrawRectangle(int x, int y, int x2, int y2, int R, int G, int B);
	void DrawRectangle(RECT Rect, int R, int G, int B);
	void DrawFallingPiece();
	void DrawNextPiece();
	void DrawGameMap();
	void DrawStats();
	void DrawGameOver();
	void DrawDebugOverlay();
	void DrawClientArea(HDC DeviceContext);
};

class windows_layer
{
public:

};

static int CountsPerSecond;                // Used to convert the timer to seconds.
static buffer* Buffer;
static game_state* GlobalGameState;
static keyboard_info* KeyboardInfo;

static bitmap_manager* BitmapManager;

struct timing_information
{
	double Seconds;
	int Counts;
	LARGE_INTEGER LICounts;
};



//bool Win32SetUpMemoryDeviceContext(HDC DeviceContext);
static void Win32AddConsole();
timing_information GetSeconds();
LRESULT Win32ProcessKeyboardMessage(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);