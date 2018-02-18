#include "Game.h"
#include <cstdlib>


game_board::game_board()
{
	this->ClearBoard();
}

void game_board::ClearBoard()
{
	for (int y = 0; y < GAME_BOARD_PLAYABLE_HEIGHT; ++y)
	{
		for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
		{
			this->GameBoard[y][x] = 0;

			// There's not really any good reason to do it like this:
			//this->SetColor(x, y, BitmapIndex::BlockBlack;
		}
	}
}



game_state::game_state()
{
	this->SetStandardPieces();
	this->AddNextPiece();
	this->NewFallingPieceAtTop();
}

void game_state::SetStandardPieces()
{
	// **   (Square)
	// **
	this->StandardPiece[0].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[0].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[0].Blocks[0].push_back(intvec2(1, 1));
	this->StandardPiece[0].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[0].Center = intvec2(0, 0);
	this->StandardPiece[0].Color = BitmapIndex::BlockWhite;
	this->StandardPiece[0].GetRotatedPiecesFrom0();

	//  *   (T)
	// ***
	this->StandardPiece[1].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[1].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[1].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[1].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[1].Center = intvec2(0, 0);
	this->StandardPiece[1].Color = BitmapIndex::BlockCyan;
	this->StandardPiece[1].GetRotatedPiecesFrom0();

	//   *  (L)
	// ***
	this->StandardPiece[2].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(1, 1));
	this->StandardPiece[2].Center = intvec2(0, 0);
	this->StandardPiece[2].Color = BitmapIndex::BlockRed;
	this->StandardPiece[2].GetRotatedPiecesFrom0();

	// *    (Backwards L)
	// ***
	this->StandardPiece[3].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[3].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[3].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[3].Blocks[0].push_back(intvec2(-1, 1));
	this->StandardPiece[3].Center = intvec2(0, 0);
	this->StandardPiece[3].Color = BitmapIndex::BlockBlue;
	this->StandardPiece[3].GetRotatedPiecesFrom0();

	//  **  (S)
	// **
	this->StandardPiece[4].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[4].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[4].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[4].Blocks[0].push_back(intvec2(1, 1));
	this->StandardPiece[4].Center = intvec2(0, 0);
	this->StandardPiece[4].Color = BitmapIndex::BlockGreen;
	this->StandardPiece[4].GetRotatedPiecesFrom0();

	// **  (Backwards S)
	//  **
	this->StandardPiece[5].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[5].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[5].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[5].Blocks[0].push_back(intvec2(-1, 1));
	this->StandardPiece[5].Center = intvec2(0, 0);
	this->StandardPiece[5].Color = BitmapIndex::BlockPurple;
	this->StandardPiece[5].GetRotatedPiecesFrom0();

	// ****  (Bar)
	// 
	this->StandardPiece[6].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[6].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[6].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[6].Blocks[0].push_back(intvec2(2, 0));
	this->StandardPiece[6].Center = intvec2(0, 0);
	this->StandardPiece[6].Color = BitmapIndex::BlockYellow;
	this->StandardPiece[6].GetRotatedPiecesFrom0();

	this->StandardPieceCount = sizeof(this->StandardPiece) / sizeof(*(this->StandardPiece));
}

void game_state::UpdateGame(keyboard_info* KeyboardInfo)
{
	if (!GameOver)
	{
		this->HandleKeyboard(KeyboardInfo);
		this->ProcessFallingPiece();
		this->UpdateLevel();
	}
}

void game_state::HandleKeyboard(keyboard_info* KeyboardInfo)
{
	KeyboardInfo->RepeatTimer = MAX(KeyboardInfo->RepeatTimer - 1.0f / TargetFPS, 0.0f);
	
	falling_piece ProposedLocation(FallingPiece);  // Copy the current falling_piece

	//intvec2 NewLocation{0,0};
	bool RepeatTimerClear = (KeyboardInfo->RepeatTimer <= 0.0f);

	this->UserIsPressingDown = KeyboardInfo->KeyDown().IsDown;

	if (KeyboardInfo->KeyLeft().IsDown == true && (RepeatTimerClear || KeyboardInfo->KeyLeft().WasDown == false))
	{
		ProposedLocation.CenterLocation = ProposedLocation.CenterLocation + intvec2(-1, 0);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (KeyboardInfo->KeyRight().IsDown == true && (RepeatTimerClear || KeyboardInfo->KeyRight().WasDown == false))
	{
		ProposedLocation.CenterLocation = ProposedLocation.CenterLocation + intvec2(1, 0);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (!ProposedLocation.HitSomething(this->GameBoard))
		FallingPiece.CenterLocation = ProposedLocation.CenterLocation;
	else
		ProposedLocation.CenterLocation = FallingPiece.CenterLocation;

	if (KeyboardInfo->KeyTurnLeft().IsDown == true && KeyboardInfo->KeyTurnLeft().WasDown == false)
	{
		ProposedLocation.PieceOrientation = MathMod(this->FallingPiece.PieceOrientation + 1, 4);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (KeyboardInfo->KeyTurnRight().IsDown == true && KeyboardInfo->KeyTurnRight().WasDown == false)
	{
		ProposedLocation.PieceOrientation = MathMod(this->FallingPiece.PieceOrientation - 1, 4);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (!ProposedLocation.HitSomething(this->GameBoard))
	{
		FallingPiece.PieceOrientation = ProposedLocation.PieceOrientation;
	}

	//if (KeyboardInfo->KeyDrop().IsDown == true && KeyboardInfo->KeyDrop().WasDown == false)
	//{
	//	this->FreezePiece();
	//	this->NewFallingPieceAtTop();
	//}
	
	if (KeyboardInfo->KeyDebug().IsDown == true && KeyboardInfo->KeyDebug().WasDown == false)
	{
		this->ShowDebugOverlay = !this->ShowDebugOverlay;
	}

	if (KeyboardInfo->KeyNextPiece().IsDown == true && KeyboardInfo->KeyNextPiece().WasDown == false)
	{
		this->NewFallingPieceAtTop();
	}

}

void game_state::ProcessFallingPiece()
{
	falling_piece& FallingPiece = this->FallingPiece;
	float Speed = this->FallSpeed;
	if (this->UserIsPressingDown)
	{
		Speed = DROP_SPEED;
	}

	if (DropTimer > 0.0f)
	{
		// It isn't time to push the piece down yet.
		DropTimer -= Speed / TargetFPS;    // TODO: Speed parameter.
	}
	else
	{
		falling_piece FPiece(FallingPiece);
		FPiece.CenterLocation = FPiece.CenterLocation + intvec2(0, -1);
		if (FPiece.HitSomething(this->GameBoard))
		{
			this->FreezePiece();
			this->NewFallingPieceAtTop();
			this->ProcessLinesAfterDrop();
		}
		else
		{
			FallingPiece.CenterLocation = this->FallingPiece.CenterLocation + intvec2(0, -1);
		}

		//FallingPiece.CenterLocation = this->FallingPiece.CenterLocation + intvec2(0, -1);
		DropTimer = 1.0f;
	}
}

void game_state::ProcessLinesAfterDrop()
{
	int LinesMade = 0;
	int PointsToAdd = 0;

	// Search from top to bottom for completed rows.
	for (int BlockY = GAME_BOARD_PLAYABLE_HEIGHT - 1; BlockY >= 0; --BlockY)
	{
		bool FoundHole = false;
		for (int BlockX = 0; BlockX < GAME_BOARD_WIDTH; ++BlockX)
		{
			if (!GameBoard.BlockHere(BlockX, BlockY))
			{
				FoundHole = true;
				break;
			}
		}

		if (!FoundHole)
		{
			// A row is complete. Drop all the blocks above it.
			for (int DroppingBlockY = BlockY; DroppingBlockY < GAME_BOARD_PLAYABLE_HEIGHT - 1; ++DroppingBlockY)
			{
				for (int DroppingBlockX = 0; DroppingBlockX < GAME_BOARD_WIDTH; ++DroppingBlockX)
				{
					// Set each block to the block above it.
					GameBoard.SetColor(DroppingBlockX, DroppingBlockY, GameBoard.GetColor(DroppingBlockX, DroppingBlockY + 1));
				}

			}

			// Clear the top row.
			for (int DroppingBlockX = 0; DroppingBlockX < GAME_BOARD_WIDTH; ++DroppingBlockX)
			{
				GameBoard.SetColor(DroppingBlockX, GAME_BOARD_PLAYABLE_HEIGHT - 1, BitmapIndex::BlockBlack);
			}

			++LinesMade;
			if (LinesMade == 1)
				PointsToAdd = 100;
			else
				PointsToAdd = PointsToAdd * 3;
			
		}
	}
	this->LineCount += LinesMade;
	this->Score += PointsToAdd;

}

void game_state::FreezePiece()
{
	falling_piece& FallingPiece = this->FallingPiece;
	int PieceOrientation = FallingPiece.PieceOrientation;
	
	bool BlockAboveLineOfDeath = false;
	auto it = FallingPiece.CurrentBlocks().begin();
	while (it != FallingPiece.CurrentBlocks().end())
	{
		intvec2 BlockLocation = FallingPiece.CenterLocation + *it;
		if (0 <= BlockLocation.x && BlockLocation.x < GAME_BOARD_WIDTH
			&& 0 <= BlockLocation.y && BlockLocation.x < GAME_BOARD_HEIGHT)
		{
			this->GameBoard.SetColor(BlockLocation.x, BlockLocation.y, FallingPiece.Color());
		}
		if (BlockLocation.y >= GAME_BOARD_HEIGHT)
		{
			this->GameOver = true;
		}
		++it;
	}
}

void game_state::AddNextPiece()
{
	int PieceIndex = (rand() % this->StandardPieceCount);
	piece* NewPiece = new piece(this->StandardPiece[PieceIndex]);
	this->NextPiece = NewPiece;
	//FallingPiece.ReplacePiece(NewPiece);

}

void game_state::NewFallingPieceAtTop()
{
	falling_piece& FallingPiece = this->FallingPiece;

	// Generate a new piece and put it in the FallingPiece.
	//int PieceIndex = (rand() % this->StandardPieceCount);
	//piece* NewPiece = new piece(this->StandardPiece[PieceIndex]);
	//FallingPiece.ReplacePiece(NewPiece);
	FallingPiece.ReplacePiece(this->NextPiece);
	int Height = FallingPiece.Piece->GetBottom();
	FallingPiece.CenterLocation = intvec2(4, Height + GAME_BOARD_HEIGHT);
	FallingPiece.PieceOrientation = 0;

	this->AddNextPiece();
	this->DropTimer = 1.0f;
}

void game_state::UpdateLevel()
{
	this->Level = (this->LineCount / 5 + 1);
	this->FallSpeed = this->Level * 4;
}

