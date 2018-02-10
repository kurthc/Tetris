#include "Game.h"




piece::piece()
{
	this->Center = intvec2(0,0);
	this->Blocks = {};
}

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
	this->FallingPiece = falling_piece(this->StandardPiece[2]);
}

void game_state::SetStandardPieces()
{
	//piece Piece {};
	this->StandardPiece[0].Blocks.push_back(intvec2(0, 0));
	this->StandardPiece[0].Blocks.push_back(intvec2(1, 0));
	this->StandardPiece[0].Blocks.push_back(intvec2(1, 1));
	this->StandardPiece[0].Blocks.push_back(intvec2(0, 1));
	this->StandardPiece[0].Center = intvec2(0, 0);
	this->StandardPiece[0].CenterType = piece_center_type::Center;

	this->StandardPiece[1].Blocks.push_back(intvec2(0, 0));
	this->StandardPiece[1].Blocks.push_back(intvec2(1, 0));
	this->StandardPiece[1].Blocks.push_back(intvec2(-1, 0));
	this->StandardPiece[1].Blocks.push_back(intvec2(0, 1));
	this->StandardPiece[1].Center = intvec2(0, 0);
	this->StandardPiece[1].CenterType = piece_center_type::Center;

	this->StandardPiece[2].Blocks.push_back(intvec2(0, 0));
	this->StandardPiece[2].Blocks.push_back(intvec2(0, -1));
	this->StandardPiece[2].Blocks.push_back(intvec2(0, -2));
	this->StandardPiece[2].Blocks.push_back(intvec2(1, -1));
	this->StandardPiece[2].Blocks.push_back(intvec2(2, 0));
	this->StandardPiece[2].Blocks.push_back(intvec2(2, -1));
	this->StandardPiece[2].Blocks.push_back(intvec2(2, -2));
	this->StandardPiece[2].Blocks.push_back(intvec2(4, 0));
	this->StandardPiece[2].Blocks.push_back(intvec2(4, -1));
	this->StandardPiece[2].Blocks.push_back(intvec2(4, -2));
	this->StandardPiece[2].Center = intvec2(0, 0);
	this->StandardPiece[2].CenterType = piece_center_type::Center;


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

	for (int i = 0; i < KeyboardInfo->size(); ++i)
	{
		if (KeyboardInfo->Key[i].VKey == 'W' && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->RepeatTimer == 0.0f)
		{
			//++this->FallingPiece.CenterLocation.y;
			NewLocation = NewLocation + intvec2(0, 1);
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		//if (KeyboardInfo->Key[i].VKey == 'A' && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->Key[i].WasDown == false)
		if (KeyboardInfo->Key[i].VKey == 'A' && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->RepeatTimer == 0.0f)
		{
			NewLocation = NewLocation + intvec2(-1, 0);
			//--this->FallingPiece.CenterLocation.x;
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		if (KeyboardInfo->Key[i].VKey == 'S' && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->RepeatTimer == 0.0f)
		{
			NewLocation = NewLocation + intvec2(0, -1);
			//--this->FallingPiece.CenterLocation.y;
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
		//if (KeyboardInfo->Key[i].VKey == 'D' && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->Key[i].WasDown == false)
		if (KeyboardInfo->Key[i].VKey == 'D' && KeyboardInfo->Key[i].IsDown == true && KeyboardInfo->RepeatTimer == 0.0f)
		{
			NewLocation = NewLocation + intvec2(1, 0);
			//++this->FallingPiece.CenterLocation.x;
			KeyboardInfo->RepeatTimer = KEYBOARD_REPEAT_TIME;
		}
	}
	//piece ProposedLocation = 
	
}