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
	if (!Success)
		std::cout << "Failure setting up Device Context\n";
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

void buffer::DrawRectangle(RECT Rect, int R, int G, int B)
{
	HBRUSH Brush;
	Brush = CreateSolidBrush(RGB(R, G, B));
	FillRect(this->MemoryDeviceContext, &Rect, Brush);
	DeleteObject(Brush);
}

void buffer::DrawRectangle(int x, int y, int x2, int y2, int R, int G, int B)
{
	RECT Rect{ x,y,x2,y2 };
	this->DrawRectangle(Rect, R, G, B);
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

	this->DrawRectangle(NextPieceRegion.left - BorderWidth, NextPieceRegion.top - BorderWidth, NextPieceRegion.right + BorderWidth, NextPieceRegion.bottom + BorderWidth, 255, 255, 255);
	this->DrawRectangle(NextPieceRegion, 0, 0, 0);

	intvec2 TopLineLeft = this->MapToDisplayCoordinates(intvec2(0, GAME_BOARD_HEIGHT + 1));
	intvec2 TopLineRight = this->MapToDisplayCoordinates(intvec2(GAME_BOARD_WIDTH, GAME_BOARD_HEIGHT + 1));
	this->DrawRectangle(TopLineLeft.x, TopLineLeft.y, TopLineRight.x, TopLineLeft.y+1, 255, 255, 255);
	//this->DrawRectangle(GAME_MAP_LEFT - BorderWidth, GAME_MA - BorderWidth)

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


void buffer::DrawStats()
{
	if (GameState != nullptr)
	{
		std::ostringstream StringBuilder;
		char LineCountString[128];
		char FPSString[128];

		// Score section:
		StringBuilder << "Lines: " << this->GameState->LineCount << std::endl;
		StringBuilder << "Level: " << this->GameState->Level << std::endl;
		StringBuilder << "Score: " << this->GameState->Score << std::endl;
		strcpy_s(LineCountString, 128, StringBuilder.str().c_str());

		// Status section: 
		StringBuilder.str("");
		StringBuilder << "Speed: " << std::setprecision(3) << GameState->FallSpeed << std::endl;
		StringBuilder << "FPS: " << std::setprecision(3) << GameState->FPSObserved << std::endl;
		strcpy_s(FPSString, 128, StringBuilder.str().c_str());


		SaveDC(this->MemoryDeviceContext);
		int DCState = SaveDC(this->MemoryDeviceContext);
		SetTextColor(this->MemoryDeviceContext, RGB(255, 255, 255));
		SetBkColor(this->MemoryDeviceContext, RGB(0, 0, 0));
		DrawText(this->MemoryDeviceContext, LineCountString, -1, (LPRECT)&ScoreRegion, DT_LEFT);
		DrawText(this->MemoryDeviceContext, FPSString, -1, (LPRECT)&StatsRegion, DT_LEFT);
		RestoreDC(this->MemoryDeviceContext, DCState);
	}

	return;
}


void buffer::DrawNextPiece()
{
	const game_board& GameBoard = this->GameState->GameBoard;
	const piece& NextPiece = *(this->GameState->NextPiece);
	int BitmapIndex = NextPiece.Color;
	std::vector<intvec2> v = ((NextPiece.Blocks)[0]);
	 
	auto it = (((NextPiece.Blocks)[0]).begin());
	while (it != (((NextPiece.Blocks)[0]).end()))
	{
		
		intvec2 BlockLocation = this->MapToDisplayCoordinates(*it)
			- intvec2(GAME_MAP_LEFT, GAME_MAP_TOP + (GameBoard.PlayableHeight) * BLOCK_HEIGHT);
		intvec2 DisplayLocation = BlockLocation + intvec2((NextPieceRegion.left + NextPieceRegion.right)/2, (NextPieceRegion.top + NextPieceRegion.bottom)/2);
		this->DrawBitmap(DisplayLocation.x, DisplayLocation.y, BLOCK_WIDTH, BLOCK_HEIGHT, this->BitmapManager->Bitmap[BitmapIndex]);
		++it;
	}

}

void buffer::DrawDebugOverlay()
{
	this->DrawRectangle(ScoreRegion, 63, 0, 0);
	this->DrawRectangle(StatsRegion, 0, 63, 0);
}


void buffer::DrawClientArea(HDC DeviceContext)
{
	if (DeviceContext != nullptr)
	{
		this->DrawRectangle(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0, 0, 0);    // Clear the client area.
		this->DrawGameMap();
		this->DrawFallingPiece();
		this->DrawNextPiece();
		this->DrawStats();
		if (this->GameState->ShowDebugOverlay)
		{
			this->DrawDebugOverlay();
		}
		BitBlt(DeviceContext, 0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, this->MemoryDeviceContext, 0, 0, SRCCOPY);
	}
	else
	{
		std::cout << "Could not find DeviceContext in buffer::DrawClientArea()\n";
	}
}