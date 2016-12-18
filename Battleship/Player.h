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
	CPlayer(const CPlayer&);
	~CPlayer();

	// Get the current win / lost or no winner state for this player
	EWIN_STATE GetWinState() const;

	// Play one turn of this player.
	// Returns true if the player should get another turn (has hit something).
	virtual bool DoTurn();

	// Display player and oppenent game boards at the specified (x, y) position.
	// Optional boolean parameter controls whether the player can see oppenents ships, or not.
	virtual void DisplayGameView(const int, const int, const bool _kbOpponentShipsVisible = false) const;
	
protected:
	CBoard& m_rBoardPlayer;
	CBoard& m_rBoardOpponent;

	mutable EWIN_STATE m_eWinState; // Cache variable
};

#endif // PLAYER_H