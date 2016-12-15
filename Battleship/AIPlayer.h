#pragma once

#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "SearchGraph.h"

#include "Player.h"

class CAIPlayer : public CPlayer
{
public:
	CAIPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent);
	~CAIPlayer();

	// Do one turn of the AI player
	void DoTurn() override;

private:
	// The Search Graph of candidate positions for ships
	CSearchGraph m_searchGraph;
};

#endif // AI_PLAYER_H