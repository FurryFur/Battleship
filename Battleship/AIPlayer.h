#pragma once

#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "SearchGraph.h"
#include "Board.h"
#include "BoardSquare.h"

#include "Player.h"

class CAIPlayer : public CPlayer
{
public:
	CAIPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent);
	CAIPlayer(const CAIPlayer&);
	~CAIPlayer();

	// Do one turn of the AI player
	bool DoTurn() override;

	void DisplayGameView(const int, const int) const override;

private:
	// The Search Graph of candidate positions for ships
	CSearchGraph m_searchGraph;
};

#endif // AI_PLAYER_H