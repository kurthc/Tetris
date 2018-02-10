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


static int CountsPerSecond;                // Used to convert the timer to seconds.
static HDC MemoryDeviceContext;
static HBITMAP MemoryDeviceContextBitmap;
static game_state* GlobalGameState;
static keyboard_info* KeyboardInfo;

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