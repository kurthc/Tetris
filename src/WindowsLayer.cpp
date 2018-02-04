#include "WindowsLayer.h"

static constexpr bool LOGMESSAGES = false;

// Get the current time from QueryPerformanceCounter().
timing_information GetSeconds()
{
	LARGE_INTEGER LICounts;
	int Counts;
	double Seconds;
	QueryPerformanceCounter(&LICounts);
	Counts = LICounts.QuadPart;
	Seconds = (double)Counts / CountsPerSecond;
	return { Seconds, Counts, LICounts };
}

// Window Procedure
static LRESULT CALLBACK WindowsTetrisWindowProcedure(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
	case(WM_PAINT):
		if (LOGMESSAGES)
			std::cout << "WM_PAINT" << "\n";
		Result = DefWindowProc(Window, Message, wParam, lParam);
		break;
	case(WM_ACTIVATE):
		if (LOGMESSAGES)
			std::cout << "WM_ACTIVATE\n";
		Result = DefWindowProc(Window, Message, wParam, lParam);
		break;
	case(WM_CLOSE):
		if (LOGMESSAGES)
			std::cout << "WM_CLOSE\n";
		DestroyWindow(Window);
		//Result = DefWindowProc(Window, Message, wParam, lParam);
		break;
	case(WM_DESTROY):
		if (LOGMESSAGES)
			std::cout << "WM_DESTROY\n";
		PostQuitMessage(0);
		break;
	default:
		if (LOGMESSAGES)
			std::cout << "Unhandled message - " << Message << "\n";
		Result = DefWindowProc(Window, Message, wParam, lParam);
	}
	
	return Result;
}


// Program entry point.
int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int Show)
{
	LARGE_INTEGER CountsPerSecondLarge;
	QueryPerformanceFrequency(&CountsPerSecondLarge);
	CountsPerSecond = (int)CountsPerSecondLarge.QuadPart;

	WNDCLASSEXA WindowClass = {};
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowsTetrisWindowProcedure;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = "TetrisMainWindow";

	RegisterClassExA(&WindowClass);
	HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "My Tetris Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		GameWindowWidth, GameWindowHeight, NULL, NULL, Instance, NULL);

	Win32AddConsole();

	HDC DeviceContext = GetDC(Window);


	bool GameLoopFinished = false;


	while (!GameLoopFinished)
	{
		timing_information TimeFrameStart = GetSeconds();

		MSG Message;
		while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE) && !GameLoopFinished)
		{
			if (Message.message == WM_QUIT)
			{
				GameLoopFinished = true;
			}
			TranslateMessage(&Message);
			DispatchMessageA(&Message);
		}
		//Sleep(15);
		timing_information TimeAfterProcess = GetSeconds();
		double ProcessingTime = TimeAfterProcess.Seconds - TimeFrameStart.Seconds;
		double TimeToSleep = 1 / TargetFPS - ProcessingTime;
		if (TimeToSleep > 0)
		{
			Sleep(1000 * TimeToSleep);
		}

		timing_information TimeFrameEnd = GetSeconds();

		std::stringstream ss{""};
		ss << "FPS: " << 1.0f / (TimeFrameEnd.Seconds - TimeFrameStart.Seconds);

		std::cout << ss.str() << "\n";

	}
}


// Attach a console window for debugging.
static void Win32AddConsole()
{
	FILE *stream;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&stream, "CON", "w", stdout);
}