#include <list>
#include <assert.h>

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

CSearchGraph::EDIRECTION CSearchGraph::GetReverseDirection(const CSearchGraph::EDIRECTION _keDirection)
{
	return static_cast<CSearchGraph::EDIRECTION>((_keDirection + 2) % 4);
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
	// If candidate node doesn't exist
	else
	{
		// Create a new candidate node at the specified position
		pCandidateNode = new CSearchNode(_krCandidatePos);

		// Add canditate node to list and lookup table
		m_listCandidateNodes.push_front(pCandidateNode);
		LookupCandidate(_krCandidatePos) = pCandidateNode;

		// Update the candidate node count
		++m_szCandidateCount;
	}

	// If an adjacent (hit) node is given
	if (_kpAdjNode != nullptr)
	{
		// Create a link to this adjacent (hit) node from the candidate node
		CSearchGraph::EDIRECTION linkDirection = _kpAdjNode->GetDirectionFrom(_krCandidatePos);
		pCandidateNode->SetAdjNode(linkDirection, _kpAdjNode);
	}

	assert(m_szCandidateCount == m_listCandidateNodes.size());
}

void CSearchGraph::AddHitNode(CSearchNode* const _kpHitNode)
{
	// Loop over all the nodes adjacent to the specified node
	for (unsigned int i = CSearchGraph::EDIRECTION::NORTH; i != CSearchGraph::EDIRECTION::NO_DIRECTION; ++i)
	{
		auto eLinkDirection = static_cast<CSearchGraph::EDIRECTION>(i);
		CSearchNode* pAdjNode = _kpHitNode->GetAdjNode(eLinkDirection);

		// Create the reverse link neccessary to connect this node back to the search graph
		if (pAdjNode != nullptr)
		{
			pAdjNode->SetAdjNode(GetReverseDirection(eLinkDirection), _kpHitNode);
		}
	}
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
				CSearchNode* pAdjNode = pCandidateNode->GetAdjNode(eLinkDirection);

				// If there two adjacent hit nodes in the graph
				if (pAdjNode != nullptr && pAdjNode->GetAdjNode(eLinkDirection) != nullptr)
				{
					assert(m_szCandidateCount == m_listCandidateNodes.size());

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
		
		assert(m_szCandidateCount == m_listCandidateNodes.size());
		
		return PopNode(it);
	}
	else
	{
		assert(m_szCandidateCount == m_listCandidateNodes.size());

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

	assert(m_szCandidateCount == m_listCandidateNodes.size());

	return pCandidateNode;
}