#include <list>

#include "Board.h"
#include "SearchNode.h"
#include "BoardPosition.hpp"

#include "SearchGraph.h"

CSearchGraph::CSearchGraph(const CBoard& _krBoard) :
m_vecCandLUTable(_krBoard.GetHeight(), std::vector<CSearchNode*>(_krBoard.GetWidth(), nullptr)),
m_szCandidateCount(0)
{
}


CSearchGraph::~CSearchGraph()
{
	
}

CSearchNode*& CSearchGraph::LookupCandidate(const TBoardPosition& _krBoardPos)
{
	return m_vecCandLUTable[_krBoardPos.m_uiRow][_krBoardPos.m_uiCol];
}

void CSearchGraph::AddCandidate(const TBoardPosition& _krCandidatePos, CSearchNode * const _kpAdjNode)
{
	// Use lookup table to check if candidate node already exists
	CSearchNode* pCandidateNode;
	CSearchNode*& rpExistingCandidate = LookupCandidate(_krCandidatePos);
	if (rpExistingCandidate)
	{
		pCandidateNode = rpExistingCandidate;
	}
	else
	{
		pCandidateNode = new CSearchNode(_krCandidatePos);
	}

	if (_kpAdjNode != nullptr)
	{
		CSearchGraph::EDIRECTION linkDirection = _kpAdjNode->GetDirectionFrom(_krCandidatePos);
		pCandidateNode->SetLink(linkDirection, _kpAdjNode);
	}

	m_listCandidateNodes.push_front(pCandidateNode);
	LookupCandidate(_krCandidatePos) = pCandidateNode;

	++m_szCandidateCount;
}

size_t CSearchGraph::GetCandidateCount() const
{
	return m_szCandidateCount;
}

CSearchNode* CSearchGraph::PopNextCandidate()
{
	// Loop over all the candidate nodes
	auto it = m_listCandidateNodes.begin();
	while (it != m_listCandidateNodes.end())
	{
		CSearchNode* pCandidateNode = *it;

		if (pCandidateNode != nullptr)
		{
			// Loop over all the adjacent hit nodes to the candidate node
			for (unsigned int i = CSearchGraph::EDIRECTION::NORTH; i != CSearchGraph::EDIRECTION::NO_DIRECTION; ++i)
			{
				auto eLinkDirection = static_cast<CSearchGraph::EDIRECTION>(i);
				CSearchNode* pAdjNode = pCandidateNode->GetLink(eLinkDirection);

				// If there two adjacent hit nodes in the graph
				if (pAdjNode != nullptr && pAdjNode->GetLink(eLinkDirection))
				{
					return PopNode(it);
				}
			}
		}

		++it;
	}

	if (m_szCandidateCount > 0)
	{
		// If we can't find a prefered candidate node then just return the first
		it = m_listCandidateNodes.begin();
		CSearchNode* pCandidateNode = *it;
		return PopNode(it);
	}
	else
	{
		// Return nullptr if there are no candidate nodes
		return nullptr;
	}
}

CSearchNode* CSearchGraph::PopNode(const std::list<CSearchNode*>::iterator& _it)
{
	CSearchNode* pCandidateNode = *_it;

	// Remove and return this candidate node from both LU table and list
	m_listCandidateNodes.erase(_it);
	TBoardPosition boardPosition = pCandidateNode->GetBoardPosition();
	LookupCandidate(boardPosition) = nullptr;

	// Decrement the candidate node count
	--m_szCandidateCount;

	return pCandidateNode;
}