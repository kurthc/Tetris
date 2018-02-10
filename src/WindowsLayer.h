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

constexpr float TargetFPS = 60.0f;

static int CountsPerSecond;
static HDC MemoryDeviceContext;
static HBITMAP MemoryDeviceContextBitmap;
static game_state* GlobalGameState;

static HBITMAP BitmapBlockPurple;
static HBITMAP BitmapBlockRed;

struct timing_information
{
	double Seconds;
	int Counts;
	LARGE_INTEGER LICounts;
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
	int size() { return Key.size(); }
};

//class key_mapping
//{
//public:
//	//std::map<int, 
//	key_mapping();
//};

static keyboard_info KeyboardInfo;
//static key_mapping KeyMapping;

bool Win32SetUpMemoryDeviceContext(HDC DeviceContext);
static void Win32AddConsole();
timing_information GetSeconds();
static void Win32DrawClientArea(HDC DeviceContext);
static void Win32DrawGameMap();
void Win32DrawRectangle(HDC DeviceContext, int x, int y, int x2, int y2, int R, int G, int B);
static void Wind32DrawFallingPiece();
LRESULT Win32ProcessKeyboardMessage(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);