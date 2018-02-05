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
	{
		if (LOGMESSAGES)
			std::cout << "WM_PAINT" << "\n";
		PAINTSTRUCT ps;
		HDC DeviceContext = BeginPaint(Window, &ps);
		Win32DrawClientArea(DeviceContext);
		EndPaint(Window, &ps);
		//Result = DefWindowProc(Window, Message, wParam, lParam);
	} break;
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
	Win32SetUpMemoryDeviceContext(DeviceContext);

	bool GameLoopFinished = false;

	GlobalGameState = new game_state();
	
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

// Create the Memory DC.
bool Win32SetUpMemoryDeviceContext(HDC DeviceContext)
{
	bool Success;
	MemoryDeviceContext = CreateCompatibleDC(DeviceContext);                             // Get a memory DC the same size/attributes as the Window one.
	MemoryDeviceContextBitmap = CreateCompatibleBitmap(DeviceContext, GameWindowWidth, GameWindowHeight);  // The memory DC needs a bitmap to write to.
	Success = SelectObject(MemoryDeviceContext, MemoryDeviceContextBitmap);                                      // Attach the bitmap to the memory DC.
	if (!Success) std::cout << "Failure setting up Device Context\n";
	return Success;
}

// Attach a console window for debugging.
static void Win32AddConsole()
{
	FILE *stream;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&stream, "CON", "w", stdout);
}

static void Win32DrawClientArea(HDC DeviceContext)
{
	Win32DrawGameMap();
	BitBlt(DeviceContext, 0, 0, GameWindowWidth, GameWindowHeight, MemoryDeviceContext, 0, 0, SRCCOPY);
}

static void Win32DrawGameMap()
{
	int GameMapLeft = 20;
	int GameMapTop = 24;
	int BlockWidth = 24;
	int BlockHeight = BlockWidth;

	game_board& GameBoard = GlobalGameState->GameBoard;

	HBRUSH Brush = CreateSolidBrush(RGB(255, 0, 0));

	RECT r = {};

	//for (int y = 0; y < 1; ++y)
	//{
	//	for (int x = 0; x < 1; ++x)
	//	{
	//		r.left = GameMapLeft + x * BlockWidth;
	//		r.top = GameMapTop + y * BlockHeight;
	//		r.right = GameMapLeft + (x + 1) * BlockWidth;
	//		r.bottom = GameMapTop + (y + 1) * BlockHeight;
	//		FillRect(MemoryDeviceContext, &r, Brush);
	//	}
	//}

	for (int y = 0; y < GameBoard.GameBoardHeight; ++y)
	{
		for (int x = 0; x < GameBoard.GameBoardWidth; ++x)
		{
			r.left = GameMapLeft + x * BlockWidth;
			r.top = GameMapTop + y * BlockHeight;
			r.right = GameMapLeft + (x + 1) * BlockWidth;
			r.bottom = GameMapTop + (y + 1) * BlockHeight;
			if (GameBoard.GameBoard[x][y] == 1)
			{
				FillRect(MemoryDeviceContext, &r, Brush);
			}
		}
	}

	DeleteObject(Brush);

	//FillRect(MemoryDeviceContext, &r, Brush);

	//r.top = 100;
	//r.bottom = 200;
	//HBRUSH Brush2 = CreateSolidBrush(RGB(255, 0, 255));
	//FillRect(MemoryDeviceContext, &r, Brush2);

	//Rectangle(MemoryDeviceContext, 0, 0, 800, 600);
	//Rectangle(MemoryDeviceContext, 0, 0, 100, 100);
}