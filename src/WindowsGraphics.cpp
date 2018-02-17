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

		RECT LinesRect;
		LinesRect.left = 10;
		LinesRect.top = 200;
		LinesRect.right = 100;
		LinesRect.bottom = 250;
		StringBuilder << "Lines: " << this->GameState->LineCount << std::endl;
		StringBuilder << "Score: " << this->GameState->Score << std::endl;
		strcpy_s(LineCountString, 128, StringBuilder.str().c_str());

		// Add an FPS indicator to the screen.
		RECT FPSRect;
		FPSRect.left = 10;
		FPSRect.top = 700;
		FPSRect.right = 100;
		FPSRect.bottom = 750;
		StringBuilder.str("");
		StringBuilder << "Speed: " << std::setprecision(3) << GameState->FallSpeed << std::endl;
		StringBuilder << "FPS: " << std::setprecision(3) << GameState->FPSObserved << std::endl;
		strcpy_s(FPSString, 128, StringBuilder.str().c_str());


		SaveDC(this->MemoryDeviceContext);
		int DCState = SaveDC(this->MemoryDeviceContext);
		SetTextColor(this->MemoryDeviceContext, RGB(255, 255, 255));
		SetBkColor(this->MemoryDeviceContext, RGB(0, 0, 0));
		DrawText(this->MemoryDeviceContext, LineCountString, -1, &LinesRect, DT_LEFT);
		DrawText(this->MemoryDeviceContext, FPSString, -1, &FPSRect, DT_LEFT);
		RestoreDC(this->MemoryDeviceContext, DCState);

		

	}



	return;
}





void buffer::DrawClientArea(HDC DeviceContext)
{
	if (DeviceContext != nullptr)
	{
		this->DrawRectangle(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0, 0, 0);    // Clear the client area.
		this->DrawGameMap();
		this->DrawFallingPiece();
		this->DrawStats();
		BitBlt(DeviceContext, 0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, this->MemoryDeviceContext, 0, 0, SRCCOPY);
	}
	else
	{
		std::cout << "Could not find DeviceContext in buffer::DrawClientArea()\n";
	}
}