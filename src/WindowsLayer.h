#pragma once

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <sstream>



constexpr int GameWindowWidth = 800;
constexpr int GameWindowHeight = 600;
constexpr float TargetFPS = 60.0f;
static int CountsPerSecond;


struct timing_information
{
	double Seconds;
	int Counts;
	LARGE_INTEGER LICounts;
};


static void Win32AddConsole();