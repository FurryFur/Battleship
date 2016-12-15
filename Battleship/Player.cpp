#include <vector>

#include "Board.h"
#include "Player.h"

CPlayer::CPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent) :
m_rBoardPlayer(_rBoardPlayer),
m_rBoardOpponent(_rBoardOpponent),
m_eWinState(CPlayer::EWIN_STATE::NO_WIN)
{
}


CPlayer::~CPlayer()
{
}

CPlayer::EWIN_STATE CPlayer::GetWinState()
{
	// Short circuit if we have already found a winner on an ealier run
	if (m_eWinState != CPlayer::EWIN_STATE::NO_WIN)
	{
		return m_eWinState;
	}

	std::vector<CShip> vecPlayerShips = m_rBoardPlayer.GetShips();
	std::vector<CShip> vecOpponentShips = m_rBoardPlayer.GetShips();

	// Check to see if the player has lost
	bool bPlayerLost = true;
	for (unsigned int i = 0; i < vecPlayerShips.size(); ++i)
	{
		if (!vecPlayerShips[i].IsDestroyed())
		{
			bPlayerLost = false;
			break;
		}
	}

	// If player has lost then the win state should be set to LOST
	if (bPlayerLost)
	{
		m_eWinState = CPlayer::EWIN_STATE::LOST;
		return m_eWinState;
	}

	// Check to see if the opponent has lost (player has won)
	bool bOpponentLost = true;
	for (unsigned int i = 0; i < vecOpponentShips.size(); ++i)
	{
		if (!vecOpponentShips[i].IsDestroyed())
		{
			bOpponentLost = false;
			break;
		}
	}

	// If the opponent has lost then the win state should be set to WIN for this player
	if (bOpponentLost)
	{
		m_eWinState = CPlayer::EWIN_STATE::WON;
		return m_eWinState;
	}

	return m_eWinState;
}

void CPlayer::DoTurn()
{

}