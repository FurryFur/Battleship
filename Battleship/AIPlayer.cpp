#include <chrono>
#include <thread>
#include <array>

#include "BoardPosition.hpp"
#include "BoardSquare.h"
#include "Board.h"
#include "Player.h"
#include "SearchNode.h"
#include "SearchGraph.h"

#include "AIPlayer.h"


CAIPlayer::CAIPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent) :
CPlayer(_rBoardPlayer, _rBoardOpponent),
m_searchGraph(_rBoardOpponent)
{
}


CAIPlayer::~CAIPlayer()
{
}

void CAIPlayer::DoTurn()
{
	// Find first candidate if none are available
	if (m_searchGraph.GetCandidateCount() == 0)
	{
		// Find a random unfired upon board position
		TBoardPosition boardPosition = m_rBoardOpponent.GetRandomBoardPosition();
		const CBoardSquare& krBoardSquare = m_rBoardOpponent.GetBoardSquare(boardPosition);
		while (krBoardSquare.GetState() != CBoardSquare::ESTATE::NOT_FIRED_UPON)
		{
			boardPosition = m_rBoardOpponent.GetRandomBoardPosition();
		}

		// Add this position as a candidate to be searched
		m_searchGraph.AddCandidate(boardPosition);
	}

	// Pop next candidate node off list
	CSearchNode* pFiredOnNode = m_searchGraph.PopNextCandidate();
	TBoardPosition firedOnPos = pFiredOnNode->GetBoardPosition();

	// Fire at will
	CBoardSquare::ESTATE eHitState = m_rBoardOpponent.FireAt(firedOnPos);
		
	// If we hit something then update the AI state
	if (eHitState == CBoardSquare::ESTATE::HIT)
	{
		// Get cardinal positions
		std::array<TBoardPosition, 4> arrCardBoardPositions = {{
				{ firedOnPos.m_uiRow - 1, firedOnPos.m_uiCol },
				{ firedOnPos.m_uiRow, firedOnPos.m_uiCol + 1 },
				{ firedOnPos.m_uiRow + 1, firedOnPos.m_uiCol },
				{ firedOnPos.m_uiRow, firedOnPos.m_uiCol - 1 }
		}};

		// Loop over each cardinal position
		for (unsigned int i = 0; i < arrCardBoardPositions.size(); ++i)
		{
			// If we can fire at the position
			TBoardPosition candidatePosition = arrCardBoardPositions[i];
			if (m_rBoardOpponent.CanFireAt(candidatePosition))
			{
				// Add this position as a new candidate with the hit square as an adjacent node
				auto eCandidateDirection = static_cast<CSearchGraph::EDIRECTION>(i);
				m_searchGraph.AddCandidate(candidatePosition, pFiredOnNode);
			}
		}
	}
	// If we didn't hit anything then we can discard this candidate
	else
	{
		delete pFiredOnNode;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
