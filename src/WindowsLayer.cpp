#include "WindowsLayer.h"

static constexpr bool LOGMESSAGES = false;
static constexpr bool LOG_FPS = false;

// Get the current time from QueryPerformanceCounter().
timing_information GetSeconds()
{
	LARGE_INTEGER LICounts;
	int Counts;
	double Seconds;
	QueryPerformanceCounter(&LICounts);
	Counts = (int)LICounts.QuadPart;
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
	{
		if (LOGMESSAGES)
			std::cout << "WM_ACTIVATE\n";
		Result = DefWindowProc(Window, Message, wParam, lParam);
	} break;

	case(WM_CLOSE):
	{
		if (LOGMESSAGES)
			std::cout << "WM_CLOSE\n";
		DestroyWindow(Window);
		//Result = DefWindowProc(Window, Message, wParam, lParam);
	} break;

	case(WM_DESTROY):
	{
		if (LOGMESSAGES)
			std::cout << "WM_DESTROY\n";
		PostQuitMessage(0);
	} break;

	case(WM_KEYDOWN):
	{
		Result = Win32ProcessKeyboardMessage(Window, Message, wParam, lParam);
	} break;

	case(WM_KEYUP):
	{
		Result = Win32ProcessKeyboardMessage(Window, Message, wParam, lParam);
	} break;

	default:
		if (LOGMESSAGES)
			std::cout << "Unhandled message - " << Message << "\n";
		Result = DefWindowProc(Window, Message, wParam, lParam);
	
	}
	
	return Result;
}

LRESULT Win32ProcessKeyboardMessage(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// At this point, Message should be either WM_KEYDOWN or WM_KEYUP
	// TODO: Include System Key Messages?
	bool IsKeyDown = (Message == WM_KEYDOWN);

	for (int i = 0; i < KeyboardInfo.size(); ++i)
	{
		if (wParam == KeyboardInfo.Key[i].VKey)
		{
			std::cout << "Setting IsKeyDown for: " << KeyboardInfo.Key[i].VKey << "\n";
			KeyboardInfo.Key[i].IsDown = IsKeyDown;
		}
	}

	return 0;
}

bool Win32HandleMessages()
{
	bool GameLoopFinished = false;
	for (int i = 0; i < KeyboardInfo.size(); ++i)
	{
		KeyboardInfo.Key[i].WasDown = KeyboardInfo.Key[i].IsDown;
	}

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
	return GameLoopFinished;
}


// Program entry point.
int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int Show)
{
	LARGE_INTEGER CountsPerSecondLarge;
	QueryPerformanceFrequency(&CountsPerSecondLarge);
	CountsPerSecond = (int)CountsPerSecondLarge.QuadPart;

	Win32AddConsole();

	WNDCLASSEXA WindowClass = {};
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowsTetrisWindowProcedure;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = "TetrisMainWindow";

	RegisterClassExA(&WindowClass);
	HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "My Tetris Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, NULL, NULL, Instance, NULL);
	
	HDC DeviceContext = GetDC(Window);
	Win32SetUpMemoryDeviceContext(DeviceContext);

	BitmapBlockPurple = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKPURPLE));

	bool GameLoopFinished = false;

	GlobalGameState = new game_state();
	
	while (!GameLoopFinished)
	{
		timing_information TimeFrameStart = GetSeconds();

		GameLoopFinished = Win32HandleMessages();
		///// Make method: Win32HandleMessages()
		//for (int i = 0; i < KeyboardInfo.size(); ++i)
		//{
		//	KeyboardInfo.Key[i].WasDown = KeyboardInfo.Key[i].IsDown;
		//}

		//MSG Message;
		//while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE) && !GameLoopFinished)
		//{
		//	if (Message.message == WM_QUIT)
		//	{
		//		GameLoopFinished = true;
		//	}
		//	TranslateMessage(&Message);
		//	DispatchMessageA(&Message);
		//}

		/////

		///// This is just for testing
		for (int i = 0; i < KeyboardInfo.size(); ++i)
		{
			if (KeyboardInfo.Key[i].VKey == 'W' && KeyboardInfo.Key[i].IsDown == true && KeyboardInfo.Key[i].WasDown == false)
			{
				++GlobalGameState->FallingPiece.CenterLocation.y;
			}
			if (KeyboardInfo.Key[i].VKey == 'A' && KeyboardInfo.Key[i].IsDown == true && KeyboardInfo.Key[i].WasDown == false)
			{
				--GlobalGameState->FallingPiece.CenterLocation.x;
			}
			if (KeyboardInfo.Key[i].VKey == 'S' && KeyboardInfo.Key[i].IsDown == true && KeyboardInfo.Key[i].WasDown == false)
			{
				--GlobalGameState->FallingPiece.CenterLocation.y;
			}
			if (KeyboardInfo.Key[i].VKey == 'D' && KeyboardInfo.Key[i].IsDown == true && KeyboardInfo.Key[i].WasDown == false)
			{
				++GlobalGameState->FallingPiece.CenterLocation.x;
			}
		}
		/////

		Win32DrawClientArea(DeviceContext);


		timing_information TimeAfterProcess = GetSeconds();
		double ProcessingTime = TimeAfterProcess.Seconds - TimeFrameStart.Seconds;
		double TimeToSleep = (1 / TargetFPS) - ProcessingTime;
		if (TimeToSleep > 0)
		{
			Sleep((DWORD)(1000 * TimeToSleep));
		}

		timing_information TimeFrameEnd = GetSeconds();

		std::stringstream ss{""};
		ss << "FPS: " << 1.0f / (TimeFrameEnd.Seconds - TimeFrameStart.Seconds);

		if (LOG_FPS)
			std::cout << ss.str() << "\n";

	}
}

// Create the Memory DC.
bool Win32SetUpMemoryDeviceContext(HDC DeviceContext)
{
	bool Success;
	MemoryDeviceContext = CreateCompatibleDC(DeviceContext);                             // Get a memory DC the same size/attributes as the Window one.
	MemoryDeviceContextBitmap = CreateCompatibleBitmap(DeviceContext, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);  // The memory DC needs a bitmap to write to.
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
	Wind32DrawFallingPiece();
	BitBlt(DeviceContext, 0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, MemoryDeviceContext, 0, 0, SRCCOPY);
}

intvec2 MapToDisplayCoordinates(intvec2 MapPosition)
{
	game_board& GameBoard = GlobalGameState->GameBoard;
	int DisplayX = GAME_MAP_LEFT + MapPosition.x * BLOCK_WIDTH;
	int DisplayY = GAME_MAP_TOP + (GameBoard.PlayableHeight - MapPosition.y) * BLOCK_HEIGHT;
	return { DisplayX, DisplayY };
}

static void Win32DrawGameMap()
{
	game_board& GameBoard = GlobalGameState->GameBoard;

	RECT r {};

	HDC BlockDC = CreateCompatibleDC(MemoryDeviceContext);
	SelectObject(BlockDC, BitmapBlockPurple);

	const static int BorderWidth = 4;
	Win32DrawRectangle(MemoryDeviceContext, GAME_MAP_LEFT - BorderWidth, GAME_MAP_TOP - BorderWidth, GAME_MAP_LEFT + GameBoard.GameBoardWidth * BLOCK_WIDTH + BorderWidth,
		GAME_MAP_TOP + GameBoard.PlayableHeight * BLOCK_HEIGHT + BorderWidth, 255, 255, 255);

	Win32DrawRectangle(MemoryDeviceContext, GAME_MAP_LEFT, GAME_MAP_TOP - 4, GAME_MAP_LEFT + GameBoard.GameBoardWidth * BLOCK_WIDTH,
		GAME_MAP_TOP + GameBoard.PlayableHeight * BLOCK_HEIGHT, 0, 0, 0);

	for (int y = GameBoard.PlayableHeight - 1; y >= 0; --y)
	{
		for (int x = 0; x < GameBoard.GameBoardWidth; ++x)
		{
			intvec2 LeftTop = MapToDisplayCoordinates(intvec2(x, y+1));
			if (GameBoard.GameBoard[x][y] == 1)
			{
				BitBlt(MemoryDeviceContext, LeftTop.x, LeftTop.y, BLOCK_WIDTH, BLOCK_HEIGHT, BlockDC, 0, 0, SRCCOPY);
			}
		}
	}

	DeleteObject(BlockDC);
}

static void Wind32DrawFallingPiece()
{
	falling_piece& FallingPiece = GlobalGameState->FallingPiece;

	HDC BlockDC = CreateCompatibleDC(MemoryDeviceContext);
	SelectObject(BlockDC, BitmapBlockPurple);

	
	std::vector<intvec2>::iterator it = FallingPiece.Piece.Blocks.begin();
	while (it != FallingPiece.Piece.Blocks.end())
	{
		intvec2 BlockLocation = MapToDisplayCoordinates(FallingPiece.CenterLocation + (*it));

		BitBlt(MemoryDeviceContext, BlockLocation.x, BlockLocation.y, BLOCK_WIDTH, BLOCK_HEIGHT, BlockDC, 0, 0, SRCCOPY);
		++it;
	}

	DeleteObject(BlockDC);
}

void Win32DrawRectangle(HDC DeviceContext, int x, int y, int x2, int y2, int R, int G, int B)
{
	RECT r{};
	HBRUSH Brush;

	Brush = CreateSolidBrush(RGB(R, G, B));
	r.left = x;
	r.top = y;
	r.right = x2;
	r.bottom = y2;
	FillRect(DeviceContext, &r, Brush);
	DeleteObject(Brush);
}


keyboard_info::keyboard_info()
{
	this->Key.push_back(key_state('W'));
	this->Key.push_back(key_state('A'));
	this->Key.push_back(key_state('S'));
	this->Key.push_back(key_state('D'));
	this->Key.push_back(key_state(VK_UP));
	this->Key.push_back(key_state(VK_LEFT));
	this->Key.push_back(key_state(VK_DOWN));
	this->Key.push_back(key_state(VK_RIGHT));
}