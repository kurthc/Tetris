#include "Global.h"
#include "WindowsLayer.h"


buffer::buffer(game_state* GameState, bitmap_manager* BitmapManager)
{
	this->GameState = GameState;
	this->BitmapManager = BitmapManager;
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


void buffer::DrawRectangle(int x, int y, int x2, int y2, int R, int G, int B)
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


void buffer::DrawBitmap(int x, int y, int width, int height, HBITMAP Bitmap)
{
	HDC BlockDC = CreateCompatibleDC(this->MemoryDeviceContext);
	SelectObject(BlockDC, Bitmap);
	BitBlt(this->MemoryDeviceContext, x, y, width, height, BlockDC, 0, 0, SRCCOPY);
	DeleteObject(BlockDC);
}


void buffer::DrawFallingPiece()
{
	falling_piece& FallingPiece = this->GameState->FallingPiece;

	int PieceOrientation = FallingPiece.PieceOrientation;
	std::vector<intvec2>::iterator it = FallingPiece.CurrentBlocks().begin();
	while (it != FallingPiece.CurrentBlocks().end())
	{
		intvec2 BlockLocation = this->MapToDisplayCoordinates(FallingPiece.CenterLocation + (*it) + intvec2(0, 1));
		int BitmapIndex = FallingPiece.Color();
		this->DrawBitmap(BlockLocation.x, BlockLocation.y, BLOCK_WIDTH, BLOCK_HEIGHT, BitmapManager->Bitmap[BitmapIndex]);
		++it;
	}

	if (this->GameState->ShowDebugOverlay)
	{
		intvec2 CenterLocation = this->MapToDisplayCoordinates(FallingPiece.CenterLocation + intvec2(0, 1));
		this->DrawRectangle(CenterLocation.x, CenterLocation.y, CenterLocation.x + BLOCK_WIDTH, CenterLocation.y + BLOCK_HEIGHT, 255, 255, 0);
	}
}


void buffer::DrawGameMap()
{
	game_board& GameBoard = this->GameState->GameBoard;

	//TODO: Make this a constant somewhere.
	const static int BorderWidth = 4;  
	this->DrawRectangle(GAME_MAP_LEFT - BorderWidth, GAME_MAP_TOP - BorderWidth, GAME_MAP_LEFT + GameBoard.GameBoardWidth * BLOCK_WIDTH + BorderWidth,
		GAME_MAP_TOP + GameBoard.PlayableHeight * BLOCK_HEIGHT + BorderWidth, 255, 255, 255);

	this->DrawRectangle(GAME_MAP_LEFT, GAME_MAP_TOP - 4, GAME_MAP_LEFT + GameBoard.GameBoardWidth * BLOCK_WIDTH,
		GAME_MAP_TOP + GameBoard.PlayableHeight * BLOCK_HEIGHT, 0, 0, 0);

	for (int y = GameBoard.PlayableHeight - 1; y >= 0; --y)
	{
		for (int x = 0; x < GameBoard.GameBoardWidth; ++x)
		{
			intvec2 LeftTop = this->MapToDisplayCoordinates(intvec2(x, y + 1));
			BitmapIndex Color = GameBoard.GetColor(x, y);
			if (Color != 0)
			{
				this->DrawBitmap(LeftTop.x, LeftTop.y, BLOCK_WIDTH, BLOCK_HEIGHT, BitmapManager->Bitmap[Color]);
			}
		}
	}
}


void buffer::DrawClientArea(HDC DeviceContext)
{
	this->DrawRectangle(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0, 0, 0);
	this->DrawGameMap();
	this->DrawFallingPiece();
	BitBlt(DeviceContext, 0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, this->MemoryDeviceContext, 0, 0, SRCCOPY);
}