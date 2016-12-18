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
	EWIN_STATE GetWinState() const;

	// Play one turn of this player.
	// Returns true if the player should get another turn (has hit something).
	virtual bool DoTurn();
	
protected:
	CBoard& m_rBoardPlayer;
	CBoard& m_rBoardOpponent;

	mutable EWIN_STATE m_eWinState; // Cache variable
};

#endif // PLAYER_H