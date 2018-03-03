#include "Game.h"
#include <cstdlib>

game_board::game_board()
{
	this->ClearBoard();
}

game_board::game_board(const game_board& InputGameBoard)
{
	for (int y = 0; y < GAME_BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
		{
			this->GameBoard[y][x] = InputGameBoard.GameBoard[y][x];
		}
	}
	//GameBoard[GAME_BOARD_PLAYABLE_HEIGHT][GAME_BOARD_WIDTH]
}


game_round::game_round(game_state* GameState)
{
	this->GameState = GameState;
}

//~game_round::game_round()
//{
//	if (this->FallingPiece)
//	{
//		delete FallingPiece;
//	}
//}


void game_board::ClearBoard()
{
	for (int y = 0; y < GAME_BOARD_PLAYABLE_HEIGHT; ++y)
	{
		for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
		{
			this->GameBoard[y][x] = 0;
		}
	}
}

bool game_board::FreezePiece(const piece& Piece, intvec2 CenterLocation, int PieceOrientation, BitmapIndex Color)
{
	bool BlockAboveLineOfDeath = false;
	auto it = Piece.Blocks[PieceOrientation].begin();
	while (it != Piece.Blocks[PieceOrientation].end())
	{
		intvec2 BlockLocation = CenterLocation + *it;
		if (0 <= BlockLocation.x && BlockLocation.x < GAME_BOARD_WIDTH
			&& 0 <= BlockLocation.y && BlockLocation.x < GAME_BOARD_HEIGHT)
		{
			this->SetColor(BlockLocation.x, BlockLocation.y, Color);
		}
		if (BlockLocation.y >= GAME_BOARD_HEIGHT)
		{
			BlockAboveLineOfDeath = true;
		}
		++it;
	}
	return BlockAboveLineOfDeath;
}

game_state::game_state()
{
	this->GameRound[0] = new game_round(this);
	this->ComputerPlayer = new computer_player(&this->GameRound[0]->GameBoard);  // This should probably be GameRound?
	this->SetStandardPieces();
	//this->GameRound.
	this->GameRound[0]->AddNextPiece();
	this->GameRound[0]->NewFallingPieceAtTop();
}

void game_state::SetStandardPieces()
{
	for (int PieceIndex = 0; PieceIndex < 7; ++PieceIndex)
	{
		piece& Piece = this->StandardPiece[PieceIndex];
		for (int BlockIndex = 0; BlockIndex < 4; ++BlockIndex)
		{
			int* a = (int*)StandardPieceBlockData[PieceIndex][BlockIndex];
			Piece.Blocks[0].push_back(intvec2(a));
		}
		Piece.Center = intvec2(0, 0);
		Piece.Color = StandardPieceColor[PieceIndex];
		Piece.GetRotatedPiecesFrom0();
		Piece.FillBounds();
	}
	
	this->StandardPieceCount = sizeof(this->StandardPiece) / sizeof(*(this->StandardPiece));
		
}

void game_state::UpdateGame(keyboard_info* KeyboardInfo)
{
	for (int i = 0; i < 1; ++i)
	{
		if (!GameOver)
		{
			if (this->Player == player::User)
			{
				this->HandleKeyboard(KeyboardInfo);
			}
			else if (this->Player == player::Computer)
			{
				this->HandleComputerKeyboard();
			}
			this->ProcessFallingPiece(this->GameRound[i]);
			this->UpdateLevel();

		}
	}
}

void game_state::HandleKeyboard(keyboard_info* KeyboardInfo)
{
	KeyboardInfo->RepeatTimer = MAX(KeyboardInfo->RepeatTimer - 1.0f / TargetFPS, 0.0f);
	
	game_round* GameRound = this->GameRound[0];
	falling_piece* CurrentFallingPiece = GameRound->FallingPiece;
	falling_piece ProposedLocation(*CurrentFallingPiece);  // Copy the current falling_piece

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

	if (!ProposedLocation.HitSomething(this->GameRound[0]->GameBoard))
		CurrentFallingPiece->CenterLocation = ProposedLocation.CenterLocation;
	else
		ProposedLocation.CenterLocation = CurrentFallingPiece->CenterLocation;

	if (KeyboardInfo->KeyTurnLeft().IsDown == true && KeyboardInfo->KeyTurnLeft().WasDown == false)
	{
		ProposedLocation.PieceOrientation = MathMod(CurrentFallingPiece->PieceOrientation + 1, 4);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (KeyboardInfo->KeyTurnRight().IsDown == true && KeyboardInfo->KeyTurnRight().WasDown == false)
	{
		ProposedLocation.PieceOrientation = MathMod(CurrentFallingPiece->PieceOrientation - 1, 4);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (!ProposedLocation.HitSomething(GameRound->GameBoard))
	{
		CurrentFallingPiece->PieceOrientation = ProposedLocation.PieceOrientation;
	}

	if (KeyboardInfo->KeyDebug().IsDown == true && KeyboardInfo->KeyDebug().WasDown == false)
	{
		this->ShowDebugOverlay = !this->ShowDebugOverlay;
	}

	if (KeyboardInfo->KeyNextPiece().IsDown == true && KeyboardInfo->KeyNextPiece().WasDown == false)
	{
		this->GameRound[0]->NewFallingPieceAtTop();
	}

}

void game_state::ProcessFallingPiece(game_round* GameRound)
{
	//falling_piece& FallingPiece = this->FallingPiece;
	falling_piece* FallingPiece = GameRound->FallingPiece;
	float Speed = this->FallSpeed;
	if (this->UserIsPressingDown)
	{
		Speed = DROP_SPEED;
	}

	if (GameRound->DropTimer > 0.0f)
	{
		// It isn't time to push the piece down yet.
		GameRound->DropTimer -= Speed / TargetFPS;
	}
	else
	{
		falling_piece FPiece(*FallingPiece);   //TODO: This is probably leaking.
		FPiece.CenterLocation = FPiece.CenterLocation + intvec2(0, -1);
		if (FPiece.HitSomething(GameRound->GameBoard))
		{
			this->FreezePiece();
			GameRound->NewFallingPieceAtTop();
			this->ProcessLinesAfterDrop();
		}
		else
		{
			FallingPiece->CenterLocation = FallingPiece->CenterLocation + intvec2(0, -1);
		}

		GameRound->DropTimer = 1.0f;
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
	this->GameOver = this->GameBoard.FreezePiece(*(FallingPiece.Piece), FallingPiece.CenterLocation, FallingPiece.PieceOrientation, FallingPiece.Color());
}




//void game_state::AddNextPiece(game_round* GameRound)
void game_round::AddNextPiece()
{
	int PieceIndex = (rand() % this->GameState->StandardPieceCount);
	piece* NewPiece = new piece(this->GameState->StandardPiece[PieceIndex]);
	this->NextPiece = NewPiece;
	//FallingPiece.ReplacePiece(NewPiece);

}

void game_round::NewFallingPieceAtTop()
{
	//falling_piece& FallingPiece = this->FallingPiece;
	this->FallingPiece->ReplacePiece(this->NextPiece);
	int Height = this->FallingPiece->Piece->GetBottom(0);
	this->FallingPiece->CenterLocation = intvec2(4, Height + GAME_BOARD_HEIGHT);
	this->FallingPiece->PieceOrientation = 0;

	this->AddNextPiece();
	this->DropTimer = 1.0f;
	if (this->GameState->Player == player::Computer)
	{
		this->GameState->ComputerPlayer->RecalculateStrategy(this->FallingPiece->Piece, this->NextPiece);
	}
}

void game_state::UpdateLevel()
{
	this->Level = (this->LineCount / 5 + 1);
	this->FallSpeed = this->Level * 4;
}

void game_state::HandleComputerKeyboard()
{
	if (this->ComputerPlayerTimer <= 0.0f)
	{
		if (this->FallingPiece.PieceOrientation != this->ComputerPlayer->StrategyOrientation)
		{
			this->FallingPiece.PieceOrientation = (this->FallingPiece.PieceOrientation + 1) % 4;
			this->ComputerPlayerTimer = 1.0f;
		}
		else if (this->FallingPiece.CenterLocation.x < this->ComputerPlayer->StrategyX)
		{
			this->FallingPiece.CenterLocation.x += 1;
			this->ComputerPlayerTimer = 1.0f;
		}
		else if (this->FallingPiece.CenterLocation.x < this->ComputerPlayer->StrategyX)
		{
			this->FallingPiece.CenterLocation.x -= 1;
			this->ComputerPlayerTimer = 1.0f;
		}
		this->UserIsPressingDown = false;
	}
	else
	{
		this->UserIsPressingDown = true;
		this->ComputerPlayerTimer -= 1.0f / TargetFPS;
	}
}


