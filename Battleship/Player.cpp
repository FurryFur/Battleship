#include "Board.h"
#include "Player.h"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

CBoard& CPlayer::GetBoard()
{
	return m_board;
}