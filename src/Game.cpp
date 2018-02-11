#include "Game.h"



game_board::game_board()
{
	for (int y = 0; y < this->GameBoardHeight; ++y)
	{
		for (int x = 0; x < this->GameBoardWidth; ++x)
		{
			if (y == 0 || y == GameBoardHeight - 1 || x == 0 || x == GameBoardWidth - 1)
			{
				this->GameBoard[x][y] = 1;
			}
			else
			{
				this->GameBoard[x][y] = 0;
			}
			
		}
	}
	
}

falling_piece::falling_piece()
{
	
}

falling_piece::falling_piece(piece Piece)
{
	this->Piece = Piece;    //copy
	this->CenterLocation = intvec2(4, 7);
	
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
	this->StandardPiece[2].Blocks[0].push_back(intvec2(0, -1));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(0, -2));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(1, -1));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(2, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(2, -1));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(2, -2));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(4, 0));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(4, -1));
	this->StandardPiece[2].Blocks[0].push_back(intvec2(4, -2));
	this->StandardPiece[2].Center = intvec2(0, 0);
	this->StandardPiece[2].CenterType = piece_center_type::Center;
	this->StandardPiece[2].GetRotatedPiecesFrom0();


	//this->StandardPiece[0].Blocks
}

void game_state::UpdateGame(keyboard_info* KeyboardInfo)
{
	this->HandleKeyboard(KeyboardInfo);
}

void game_state::HandleKeyboard(keyboard_info* KeyboardInfo)
{
	KeyboardInfo->RepeatTimer = MAX(KeyboardInfo->RepeatTimer - 1.0f / TargetFPS, 0.0f);
	intvec2 NewLocation{0,0};
	bool RepeatTimerClear = (KeyboardInfo->RepeatTimer <= 0.0f);

	for (int i = 0; i < KeyboardInfo->size(); ++i)
	{
		
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
		}
	}

	// For now...
	if (NewLocation.x != 0 || NewLocation.y != 0)
	{
		this->FallingPiece.CenterLocation = this->FallingPiece.CenterLocation + NewLocation;
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