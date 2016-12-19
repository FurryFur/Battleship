//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name   : AIPlayer.h
// Description : Header file for CAIPlayer class
// Author      : Lance Chaney
// Mail        : lance.cha7337@mediadesign.school.nz
//

#pragma once

#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "SearchGraph.h"
#include "Board.h"
#include "BoardSquare.h"

#include "Player.h"

// Subclass of CPlayer.
// Implements AI controlled behavior.
class CAIPlayer : public CPlayer
{
public:
	CAIPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent);
	CAIPlayer(const CAIPlayer&);
	~CAIPlayer();

	// Do one turn of the AI player.
	// Returns true if the AI player should get another turn (has hit something).
	bool DoTurn() override;

	// Display AI player and oppenent game boards at the specified (x, y) position.
	// Optional boolean parameter controls whether the player can see oppenents ships.
	void DisplayGameView(const int, const int, const bool _kbOpponentShipsVisible = false) const override;

// TF: Access Specifier
private:
	// The Search Graph of candidate positions for ships
	CSearchGraph m_searchGraph;
};

#endif // AI_PLAYER_H