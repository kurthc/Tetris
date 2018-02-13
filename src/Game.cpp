#include "Game.h"
#include <cstdlib>


game_board::game_board()
{
	for (int y = 0; y < GAME_BOARD_PLAYABLE_HEIGHT; ++y)
	{
		for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
		{
			this->GameBoard[x][y] = 0;
		}
	}
	
}

falling_piece::falling_piece()
{
	
}

falling_piece::falling_piece(piece Piece)
{
	this->Piece = Piece;    //copy
	int Height = Piece.GetBottom();
	this->CenterLocation = intvec2(4, Height + GAME_BOARD_HEIGHT);
	
}

game_state::game_state()
{
	this->SetStandardPieces();
	this->FallingPiece = falling_piece(this->StandardPiece[1]);
}

void game_state::SetStandardPieces()
{
	//piece Piece {};
	this->StandardPiece[0].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[0].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[0].Blocks[0].push_back(intvec2(1, 1));
	this->StandardPiece[0].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[0].Center = intvec2(0, 0);
	this->StandardPiece[0].CenterType = piece_center_type::Center;
	this->StandardPiece[0].GetRotatedPiecesFrom0();

	this->StandardPiece[1].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[1].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[1].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[1].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[1].Center = intvec2(0, 0);
	this->StandardPiece[1].CenterType = piece_center_type::Center;
	this->StandardPiece[1].GetRotatedPiecesFrom0();

	this->StandardPiece[2].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(1, 1));
	this->StandardPiece[2].Center = intvec2(0, 0);
	this->StandardPiece[2].CenterType = piece_center_type::Center;
	this->StandardPiece[2].GetRotatedPiecesFrom0();

	this->StandardPiece[3].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[3].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[3].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[3].Blocks[0].push_back(intvec2(-1, 1));
	this->StandardPiece[3].Center = intvec2(0, 0);
	this->StandardPiece[3].CenterType = piece_center_type::Center;
	this->StandardPiece[3].GetRotatedPiecesFrom0();

	this->StandardPiece[4].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[4].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[4].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[4].Blocks[0].push_back(intvec2(1, 1));
	this->StandardPiece[4].Center = intvec2(0, 0);
	this->StandardPiece[4].CenterType = piece_center_type::Center;
	this->StandardPiece[4].GetRotatedPiecesFrom0();

	this->StandardPiece[5].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[5].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[5].Blocks[0].push_back(intvec2(0, 1));
	this->StandardPiece[5].Blocks[0].push_back(intvec2(-1, 1));
	this->StandardPiece[5].Center = intvec2(0, 0);
	this->StandardPiece[5].CenterType = piece_center_type::Center;
	this->StandardPiece[5].GetRotatedPiecesFrom0();

	this->StandardPiece[6].Blocks[0].push_back(intvec2(0, 0));
	this->StandardPiece[6].Blocks[0].push_back(intvec2(-1, 0));
	this->StandardPiece[6].Blocks[0].push_back(intvec2(1, 0));
	this->StandardPiece[6].Blocks[0].push_back(intvec2(2, 0));
	this->StandardPiece[6].Center = intvec2(0, 0);
	this->StandardPiece[6].CenterType = piece_center_type::Center;
	this->StandardPiece[6].GetRotatedPiecesFrom0();

	this->StandardPieceCount = sizeof(this->StandardPiece) / sizeof(*(this->StandardPiece));
	//this->StandardPiece[0].Blocks
}

void game_state::UpdateGame(keyboard_info* KeyboardInfo)
{
	this->HandleKeyboard(KeyboardInfo);
	this->ProcessFallingPiece();
}

void game_state::HandleKeyboard(keyboard_info* KeyboardInfo)
{
	KeyboardInfo->RepeatTimer = MAX(KeyboardInfo->RepeatTimer - 1.0f / TargetFPS, 0.0f);
	intvec2 NewLocation{0,0};
	bool RepeatTimerClear = (KeyboardInfo->RepeatTimer <= 0.0f);

	this->UserIsPressingDown = KeyboardInfo->KeyDown().IsDown;

	if (KeyboardInfo->KeyLeft().IsDown == true && RepeatTimerClear)
	{
		NewLocation = NewLocation + intvec2(-1, 0);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (KeyboardInfo->KeyRight().IsDown == true && RepeatTimerClear)
	{
		NewLocation = NewLocation + intvec2(1, 0);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	falling_piece NewFallingPiece(FallingPiece);
	NewFallingPiece.CenterLocation = NewFallingPiece.CenterLocation + NewLocation;
	if (!NewFallingPiece.HitSomething(this->GameBoard))
	{
		FallingPiece.CenterLocation = FallingPiece.CenterLocation + NewLocation;
	}

	int NewOrientation = this->FallingPiece.PieceOrientation;
	if (KeyboardInfo->KeyTurnLeft().IsDown == true && KeyboardInfo->KeyTurnLeft().WasDown == false)
	{
		NewOrientation = ProperMod(this->FallingPiece.PieceOrientation + 1, 4);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	if (KeyboardInfo->KeyTurnRight().IsDown == true && KeyboardInfo->KeyTurnRight().WasDown == false)
	{
		NewOrientation = ProperMod(this->FallingPiece.PieceOrientation - 1, 4);
		KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
	}

	falling_piece NewFallingPiece2(FallingPiece);
	NewFallingPiece2.PieceOrientation = NewOrientation;
	if (!NewFallingPiece2.HitSomething(this->GameBoard))
	{
		FallingPiece.PieceOrientation = NewOrientation;
	}

	if (KeyboardInfo->KeyDrop().IsDown == true && KeyboardInfo->KeyDrop().WasDown == false)
	{
		this->FreezePiece();
		this->NewFallingPieceAtTop();
	}
	
	if (KeyboardInfo->KeyDebug().IsDown == true && KeyboardInfo->KeyDebug().WasDown == false)
	{
		this->ShowDebugOverlay = !this->ShowDebugOverlay;
	}

	/*
	for (int i = 0; i < KeyboardInfo->size(); ++i)
	{
		//TODO: I think each key needs its own RepeatTimer.
		if (KeyboardInfo->Key[i].VKey == 'W' && KeyboardInfo->Key[i].IsDown == true && RepeatTimerClear)
		{
			NewLocation = NewLocation + intvec2(0, 1);
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		if (KeyboardInfo->Key[i].VKey == 'A' && KeyboardInfo->Key[i].IsDown == true && RepeatTimerClear)
		{
			NewLocation = NewLocation + intvec2(-1, 0);
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		if (KeyboardInfo->Key[i].VKey == 'S' && KeyboardInfo->Key[i].IsDown == true && RepeatTimerClear)
		{
			NewLocation = NewLocation + intvec2(0, -1);
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		if (KeyboardInfo->Key[i].VKey == 'D' && KeyboardInfo->Key[i].IsDown == true && RepeatTimerClear)
		{
			NewLocation = NewLocation + intvec2(1, 0);
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		if (KeyboardInfo->Key[i].VKey == 'J' && KeyboardInfo->Key[i].IsDown == true && RepeatTimerClear)
		{
			this->FallingPiece.PieceOrientation = ProperMod(this->FallingPiece.PieceOrientation + 1, 4);
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		if (KeyboardInfo->Key[i].VKey == 'L' && KeyboardInfo->Key[i].IsDown == true && RepeatTimerClear)
		{
			this->FallingPiece.PieceOrientation = ProperMod(this->FallingPiece.PieceOrientation - 1, 4);
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		if (KeyboardInfo->Key[i].VKey == VK_SPACE && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->Key[i].WasDown == false)
		{
			this->FreezePiece();
			this->NewFallingPieceAtTop();
		}
		if (KeyboardInfo->Key[i].VKey == '1' && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->Key[i].WasDown == false)
		{
			this->ShowDebugOverlay = !this->ShowDebugOverlay;
		}
	}
	*/

	//// For now...
	//if (NewLocation.x != 0 || NewLocation.y != 0)
	//{
	//	this->FallingPiece.CenterLocation = this->FallingPiece.CenterLocation + NewLocation;
	//}
}

void game_state::ProcessFallingPiece()
{
	falling_piece& FallingPiece = this->FallingPiece;
	float Speed = 5.0f;
	if (this->UserIsPressingDown)
	{
		Speed = 20.0f;
	}

	if (DropTimer > 0.0f)
	{
		// It isn't time to push the piece down yet.
		DropTimer -= Speed / TargetFPS;    // TODO: Speed parameter.
	}
	else
	{
		//falling_piece FPiece(FallingPiece.Piece);
		falling_piece FPiece(FallingPiece);
		FPiece.CenterLocation = FPiece.CenterLocation + intvec2(0, -1);
		if (FPiece.HitSomething(this->GameBoard))
		{
			this->FreezePiece();
			this->NewFallingPieceAtTop();
		}
		else
		{
			FallingPiece.CenterLocation = this->FallingPiece.CenterLocation + intvec2(0, -1);
		}

		//FallingPiece.CenterLocation = this->FallingPiece.CenterLocation + intvec2(0, -1);
		DropTimer = 1.0f;
	}

	for (int i = 0; i < GAME_BOARD_PLAYABLE_HEIGHT; ++i)
	{
		bool FoundHole = false;
		for (int j = 0; j < GAME_BOARD_WIDTH; ++j)
		{
			//TODO: Why is Width the first value here?
			if (GameBoard.GameBoard[j][i] == 0)
			{
				//TODO: break
				FoundHole = true;
			}
		}
		if (!FoundHole)
		{
			for (int k = i; k < GAME_BOARD_PLAYABLE_HEIGHT - 1; ++k)
			{
				for (int l = 0; l < GAME_BOARD_WIDTH; ++l)
				{
					GameBoard.GameBoard[l][k] = GameBoard.GameBoard[l][k + 1];
				}
				
			}
		}

	}


}


void game_state::FreezePiece()
{
	falling_piece& FallingPiece = this->FallingPiece;
	int PieceOrientation = FallingPiece.PieceOrientation;
	auto it = FallingPiece.Piece.Blocks[PieceOrientation].begin();
	while (it != FallingPiece.Piece.Blocks[PieceOrientation].end())
	{
		intvec2 BlockLocation = FallingPiece.CenterLocation + *it;
		if (0 <= BlockLocation.x && BlockLocation.x < GAME_BOARD_WIDTH
			&& 0 <= BlockLocation.y && BlockLocation.x < GAME_BOARD_HEIGHT)
		{
			this->GameBoard.GameBoard[BlockLocation.x][BlockLocation.y] = 1;
		}
		++it;
	}

}

// I don't know if I'll need this anywhere anymore.
intvec2 falling_piece::BlockPosition(int n)
{
	int PieceOrientation = this->PieceOrientation;
	return this->Piece.Blocks[PieceOrientation][n] + this->CenterLocation;
}

void game_state::NewFallingPieceAtTop()
{
	int PieceIndex = (rand() % this->StandardPieceCount);
	this->FallingPiece = falling_piece(this->StandardPiece[PieceIndex]);
	this->DropTimer = 1.0f;
}

bool falling_piece::HitSomething(const game_board& GameBoard)
{
	bool IsOverlapping = false;
	//for (int i = 0; i < this->Blocks().size(); ++i)
	//{
	//	intvec2 b = this->Blocks()[i];
	//	if (GameBoard.GameBoard[b.x][b.y] != 0)
	//	{
	//		IsOverlapping = true;
	//	}
	//}

	for (int i = 0; i < this->Blocks().size(); ++i)
	{
		intvec2 b = (*this)[i];
		if (b.x < 0 || b.x >= GAME_BOARD_WIDTH || b.y < 0)
		{
			IsOverlapping = true;
		}
		else if (GameBoard.GameBoard[b.x][b.y] != 0)
		{
			IsOverlapping = true;
		}
	}

	
	return IsOverlapping;
}

// GameMap coordinates of the nth block.
intvec2 falling_piece::operator[](const int& n)
{
	return (this->Blocks())[n] + this->CenterLocation;
}