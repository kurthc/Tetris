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
	std::cout << "LiCounts = " << LICounts.HighPart << " and " << LICounts.LowPart << "\n";
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
		Buffer->DrawClientArea(DeviceContext);
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

	for (int i = 0; i < KeyboardInfo->size(); ++i)
	{
		if (wParam == KeyboardInfo->Key[i].VKey)
		{
			std::cout << "Setting IsKeyDown for: " << KeyboardInfo->Key[i].VKey << "\n";
			KeyboardInfo->Key[i].IsDown = IsKeyDown;
		}
	}

	return 0;
}

bool Win32HandleMessages()
{
	// Update WasDown.
	bool GameLoopFinished = false;
	for (int i = 0; i < KeyboardInfo->size(); ++i)
	{
		KeyboardInfo->Key[i].WasDown = KeyboardInfo->Key[i].IsDown;
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

	bool GameLoopFinished = false;

	GlobalGameState = new game_state();
	BitmapManager = new bitmap_manager();
	BitmapManager->LoadBitmaps(Instance);
	Buffer = new buffer(GlobalGameState, BitmapManager);
	Buffer->SetUpMemoryDeviceContext(DeviceContext);
	KeyboardInfo = new keyboard_info();
	
	
	while (!GameLoopFinished)
	{
		timing_information TimeFrameStart = GetSeconds();

		GameLoopFinished = Win32HandleMessages();

		GlobalGameState->UpdateGame(KeyboardInfo);


		Buffer->DrawClientArea(DeviceContext);


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



// Attach a console window for debugging.
static void Win32AddConsole()
{
	FILE *stream;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&stream, "CON", "w", stdout);
}


keyboard_info::keyboard_info()
{
	
	std::pair<WPARAM, KeyFunction> KeysToAdd[] = {
		{'W', KeyFunction::Unmapped},
		{'A', KeyFunction::Left},
		{'S', KeyFunction::Unmapped},
		{'D', KeyFunction::Right},
		{'J', KeyFunction::TurnLeft},
		{'L', KeyFunction::TurnRight},
		{'1', KeyFunction::Debug},
		{VK_SPACE, KeyFunction::Drop}

	};

	int NumberOfKeys = sizeof(KeysToAdd) / sizeof(*KeysToAdd);
	for (int i = 0; i < NumberOfKeys; ++i)
	{
		this->Key.push_back(key_state(KeysToAdd[i].first));
		switch (KeysToAdd[i].second)
		{
		case KeyFunction::Left:
			this->IndexLeft = this->Key.size() - 1;
			break;
		case KeyFunction::Right:
			this->IndexRight = this->Key.size() - 1;
			break;
		case KeyFunction::TurnLeft:
			this->IndexTurnLeft = this->Key.size() - 1;
			break;
		case KeyFunction::TurnRight:
			this->IndexTurnRight = this->Key.size() - 1;
			break;
		case KeyFunction::Drop:
			this->IndexDrop = this->Key.size() - 1;
			break;
		case KeyFunction::Debug:
			this->IndexDebug = this->Key.size() - 1;
			break;
		}
	}
}

//bitmap_manager::bitmap_manager(HINSTANCE Instance)
void bitmap_manager::LoadBitmaps(HINSTANCE Instance)
{
	//this->Bitmap = std::vector<BITMAP>();
	//BitmapIndex::BlockBlack
	this->Bitmap[BitmapIndex::BlockBlack] = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKBLACK));
	this->Bitmap[BitmapIndex::BlockPurple] = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKPURPLE));
	this->Bitmap[BitmapIndex::BlockBlue] = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKBLUE));
	this->Bitmap[BitmapIndex::BlockGreen] = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKGREEN));
	this->Bitmap[BitmapIndex::BlockRed] = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKRED));
	this->Bitmap[BitmapIndex::BlockYellow] = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKYELLOW));

	//BitmapBlockPurple = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKPURPLE));
	//BitmapBlockRed = LoadBitmap(Instance, MAKEINTRESOURCE(IDB_BLOCKRED));
}