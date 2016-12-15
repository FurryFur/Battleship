#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Board.h"
#include "Ship.h"

class CPlayer
{
public:
	enum EWIN_STATE
	{
		WON,
		LOST,
		NO_WIN
	};

	CPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent);
	~CPlayer();

	// Get the current win / lost or no winner state for this player
	EWIN_STATE GetWinState();

	virtual void DoTurn();
	
protected:
	CBoard& m_rBoardPlayer;
	CBoard& m_rBoardOpponent;

	EWIN_STATE m_eWinState;
};

#endif // PLAYER_H