#include "Global.h"
#include "WindowsLayer.h"


buffer::buffer(game_state* GameState)
{
	this->GameState = GameState;
}

// Create the Memory DC.
//bool Win32SetUpMemoryDeviceContext(HDC DeviceContext)
bool buffer::SetUpMemoryDeviceContext(HDC WindowDeviceContext)
{
	bool Success;
	MemoryDeviceContext = CreateCompatibleDC(WindowDeviceContext);                             // Get a memory DC the same size/attributes as the Window one.
	MemoryDeviceContextBitmap = CreateCompatibleBitmap(WindowDeviceContext, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);  // The memory DC needs a bitmap to write to.
	Success = SelectObject(MemoryDeviceContext, MemoryDeviceContextBitmap);                                      // Attach the bitmap to the memory DC.
	if (!Success) std::cout << "Failure setting up Device Context\n";
	return Success;
}


intvec2 buffer::MapToDisplayCoordinates(intvec2 MapPosition)
{
	game_board& GameBoard = this->GameState->GameBoard;
	//game_board& GameBoard = this->GameState.GameBoard;
	int DisplayX = GAME_MAP_LEFT + MapPosition.x * BLOCK_WIDTH;
	int DisplayY = GAME_MAP_TOP + (GameBoard.PlayableHeight - MapPosition.y) * BLOCK_HEIGHT;
	return { DisplayX, DisplayY };
}


void buffer::Win32DrawRectangle(int x, int y, int x2, int y2, int R, int G, int B)
{
	RECT r{};
	HBRUSH Brush;

	Brush = CreateSolidBrush(RGB(R, G, B));
	r.left = x;
	r.top = y;
	r.right = x2;
	r.bottom = y2;
	FillRect(this->MemoryDeviceContext, &r, Brush);
	DeleteObject(Brush);
}
