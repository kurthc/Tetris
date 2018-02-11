#include "Global.h"
#include "WindowsLayer.h"


buffer::buffer(game_state* GameState)
{
	this->GameState = GameState;
}

// Create the Memory DC.
//bool Win32SetUpMemoryDeviceContext(HDC DeviceContext)
bool buffer::SetUpMemoryDeviceContext(HDC DeviceContext)
{
	bool Success;
	MemoryDeviceContext = CreateCompatibleDC(DeviceContext);                             // Get a memory DC the same size/attributes as the Window one.
	MemoryDeviceContextBitmap = CreateCompatibleBitmap(DeviceContext, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);  // The memory DC needs a bitmap to write to.
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