#include <chrono>
#include <thread>

#include "BoardPosition.hpp"
#include "BoardSquare.h"
#include "Board.h"
#include "Player.h"

#include "AIPlayer.h"


CAIPlayer::CAIPlayer(CBoard& _rBoardPlayer, CBoard& _rBoardOpponent) : 
CPlayer(_rBoardPlayer, _rBoardOpponent)
{
}


CAIPlayer::~CAIPlayer()
{
}

void CAIPlayer::DoTurn()
{
	//if (m_searchGraph.GetSize() > 0)
	//{
	//	
	//}
	//else
	//{
		// Find an unfired upon board position
		TBoardPosition boardPosition = m_rBoardOpponent.GetRandomBoardPosition();
		const CBoardSquare& krBoardSquare = m_rBoardOpponent.GetBoardSquare(boardPosition);
		while (krBoardSquare.GetState() != CBoardSquare::ESTATE::NOT_FIRED_UPON)
		{
			boardPosition = m_rBoardOpponent.GetRandomBoardPosition();
		}

		// Fire at will
		CBoardSquare::ESTATE eHitState = m_rBoardOpponent.FireAt(boardPosition);
		
		// If we hit something then update the AI state
		if (eHitState == CBoardSquare::ESTATE::HIT)
		{
			m_searchGraph.AddSearchPosition(boardPosition);

			// Get cardinal positions
			TBoardPosition northPos, southPos, eastPos, westPos;
			northPos = southPos = eastPos = westPos = boardPosition;
			--northPos.m_uiRow;
			++southPos.m_uiRow;
			++eastPos.m_uiCol;
			--westPos.m_uiCol;

			if (m_rBoardOpponent.CanFireAt(northPos))
			{
				m_searchGraph.A
			}
		}
	//}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
