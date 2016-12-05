#include "Board.h"
#include "Player.h"

size_t kBOARD_SIZE = 10;

CPlayer::CPlayer() :
m_board(kBOARD_SIZE, kBOARD_SIZE)
{
}


CPlayer::~CPlayer()
{
}

CBoard& CPlayer::GetBoard()
{
	return m_board;
}