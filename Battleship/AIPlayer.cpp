//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name   : AIPlayer.cpp
// Description : Implementation file for CAIPlayer class.
// Author      : Lance Chaney
// Mail        : lance.cha7337@mediadesign.school.nz
//

#include <chrono>
#include <thread>
#include <array>
#include <assert.h>
#include <iostream>

#include "BoardPosition.hpp"
#include "BoardSquare.h"
#include "Board.h"
#include "Player.h"
#include "SearchNode.h"
#include "SearchGraph.h"
#include "Util.h"

#include "AIPlayer.h"


CAIPlayer::CAIPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent) :
CPlayer(_rBoardPlayer, _rBoardOpponent),
m_searchGraph(_rBoardOpponent)
{
}

CAIPlayer::CAIPlayer(const CAIPlayer& _krRhs) :
CPlayer(_krRhs),
m_searchGraph(_krRhs.m_searchGraph)
{
}


CAIPlayer::~CAIPlayer()
{
}

void CAIPlayer::DisplayGameView(const int _kiX, const int _kiY, const bool _kbOpponentShipsVisible) const
{
	Util::GotoXY(_kiX, _kiY);
	std::cout << "               Computer's View:";
	CPlayer::DisplayGameView(_kiX, _kiY + 1, _kbOpponentShipsVisible);
}

bool CAIPlayer::DoTurn()
{
	// Find first candidate if none are available
	if (m_searchGraph.GetCandidateCount() == 0)
	{
		// Find a random unfired upon board position
		TBoardPosition boardPosition = m_rBoardOpponent.GetRandomBoardPosition();
		while (m_rBoardOpponent.GetBoardSquare(boardPosition).GetState() != CBoardSquare::ESTATE::NOT_FIRED_UPON)
		{
			boardPosition = m_rBoardOpponent.GetRandomBoardPosition();
		}

		// Add this position as a candidate to be searched
		m_searchGraph.AddCandidate(boardPosition);
	}

	// Pop next candidate node off list
	CSearchNode* pFiredOnNode = m_searchGraph.PopNextCandidate();
	TBoardPosition firedOnPos = pFiredOnNode->GetBoardPosition();

	assert(m_rBoardOpponent.GetBoardSquare(firedOnPos).GetState() == CBoardSquare::ESTATE::NOT_FIRED_UPON);

	// Fire at will
	CBoardSquare::ESTATE eHitState = m_rBoardOpponent.FireAt(firedOnPos);

	assert(m_rBoardOpponent.GetBoardSquare(firedOnPos).GetState() != CBoardSquare::ESTATE::NOT_FIRED_UPON);
		
	// If we hit something then update the AI state
	if (eHitState == CBoardSquare::ESTATE::HIT)
	{
		// Add the fired on node back to the search graph as a hit node
		m_searchGraph.AddHitNode(pFiredOnNode);

		// Get cardinal positions
		TBoardPosition arrCardPos[4];
		CBoard::FillWithCardinalPositions(firedOnPos, arrCardPos);

		// Loop over each cardinal position
		for (unsigned int i = 0; i < 4; ++i)
		{
			// If we can fire at the position
			if (m_rBoardOpponent.CanFireAt(arrCardPos[i]))
			{
				// Add this position as a new candidate with the hit node as an adjacent node
				auto eCandidateDirection = static_cast<CSearchGraph::EDIRECTION>(i);
				m_searchGraph.AddCandidate(arrCardPos[i], pFiredOnNode);
			}
		}
	}
	// If we destroyed something then update the AI state
	else if (eHitState == CBoardSquare::ESTATE::DESTROYED)
	{
		// Add the fired on node back to the search graph as a hit node
		m_searchGraph.AddHitNode(pFiredOnNode);

		// Prune the graph starting at the fired on node.
		// The prune condition for nodes is that they must corrospond to a destroyed
		// board square.
		m_searchGraph.PruneGraph(
			pFiredOnNode, 
			[&] (CSearchNode const * const _kpkSearchNode) 
			{ 
				TBoardPosition nodeBoardPosition = _kpkSearchNode->GetBoardPosition();
				CBoardSquare nodeBoardSquare = m_rBoardOpponent.GetBoardSquare(nodeBoardPosition);
				return nodeBoardSquare.GetState() == CBoardSquare::ESTATE::DESTROYED;
			}
		);
	}
	// If we didn't hit anything then we can discard this candidate
	else
	{
		delete pFiredOnNode;
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));

	// Should the ai player get another turn?
	return HasAnotherTurn(eHitState);
}
