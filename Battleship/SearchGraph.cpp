#include <list>
#include <assert.h>
#include <functional>
#include <random>

#include "Util.h"
#include "Board.h"
#include "SearchNode.h"
#include "BoardPosition.hpp"

#include "SearchGraph.h"

CSearchGraph::CSearchGraph(const CBoard& _krBoard) :
m_vecNodeLUTable(_krBoard.GetHeight(), std::vector<CSearchNode*>(_krBoard.GetWidth(), nullptr)),
m_szNodeCount(0),
m_szLUTWidth(_krBoard.GetWidth()),
m_szLUTHeight(_krBoard.GetHeight())
{
}


CSearchGraph::~CSearchGraph()
{
	// Prune graph starting at candidate nodes until there are none left
	while (m_listCandidateNodes.begin() != m_listCandidateNodes.end())
	{
		PruneGraph(*m_listCandidateNodes.begin(), nullptr);
	}
}

CSearchGraph::EDIRECTION CSearchGraph::GetReverseDirection(const CSearchGraph::EDIRECTION _keDirection)
{
	return static_cast<CSearchGraph::EDIRECTION>((_keDirection + 2) % 4);
}

bool CSearchGraph::IsValidLUPosition(const TBoardPosition& _krPosition) const
{
	// Check if position is on the board
	if (_krPosition.m_uiRow < m_szLUTHeight && _krPosition.m_uiCol < m_szLUTWidth)
	{
		return true;
	}

	return false;
}

CSearchNode*& CSearchGraph::LookupNode(const TBoardPosition& _krBoardPos)
{
	return m_vecNodeLUTable[_krBoardPos.m_uiRow][_krBoardPos.m_uiCol];
}

void CSearchGraph::AddCandidate(const TBoardPosition& _krCandidatePos, CSearchNode * const _kpAdjNode)
{
	// Use lookup table to check if candidate node already exists
	CSearchNode* pCandidateNode;
	CSearchNode*& rpExistingCandidate = LookupNode(_krCandidatePos);
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
		LookupNode(_krCandidatePos) = pCandidateNode;

		// Update the total node count
		++m_szNodeCount;
	}

	// If an adjacent (hit) node is given
	if (_kpAdjNode != nullptr)
	{
		// Create a link to this adjacent (hit) node from the candidate node
		CSearchGraph::EDIRECTION linkDirection = _kpAdjNode->GetDirectionFrom(_krCandidatePos);
		pCandidateNode->SetAdjHitNode(linkDirection, _kpAdjNode);
	}

	// Set this node as a candidate node
	pCandidateNode->SetCandidateNode();
}

void CSearchGraph::AddHitNode(CSearchNode* const _kpHitNode)
{
	// Set this node as a hit node
	_kpHitNode->SetHitNode();

	// Add the node back to the lookup table
	LookupNode(_kpHitNode->GetBoardPosition()) = _kpHitNode;

	// Increment the total node count
	++m_szNodeCount;

	// Loop over all the nodes adjacent to the specified node
	for (unsigned int i = CSearchGraph::EDIRECTION::NORTH; i != CSearchGraph::EDIRECTION::NO_DIRECTION; ++i)
	{
		auto eLinkDirection = static_cast<CSearchGraph::EDIRECTION>(i);
		CSearchNode* pAdjNode = _kpHitNode->GetAdjHitNode(eLinkDirection);

		// Create the reverse link neccessary to connect this node back to the search graph
		if (pAdjNode != nullptr)
		{
			pAdjNode->SetAdjHitNode(GetReverseDirection(eLinkDirection), _kpHitNode);
		}
	}
}

size_t CSearchGraph::GetCandidateCount() const
{
	return m_listCandidateNodes.size();
}

CSearchNode* CSearchGraph::PopNextCandidate()
{
	// Check we have candidate nodes
	if (GetCandidateCount() > 0)
	{
		// Loop over all the candidate nodes
		for (auto it = m_listCandidateNodes.begin(); it != m_listCandidateNodes.end(); ++it)
		{
			CSearchNode* pCandidateNode = *it;

			if (pCandidateNode != nullptr)
			{
				// Loop over all the adjacent hit nodes to the candidate node
				for (unsigned int i = CSearchGraph::EDIRECTION::NORTH; i != CSearchGraph::EDIRECTION::NO_DIRECTION; ++i)
				{
					auto eLinkDirection = static_cast<CSearchGraph::EDIRECTION>(i);
					CSearchNode* pAdjNode = pCandidateNode->GetAdjHitNode(eLinkDirection);

					// If there two adjacent hit nodes in the graph
					if (pAdjNode != nullptr && pAdjNode->GetAdjHitNode(eLinkDirection) != nullptr)
					{
						return PopCandidateNode(it);
					}
				}
			}
		}
	
		// If we can't find a prefered candidate node then return one at random
		auto it = m_listCandidateNodes.begin();
		auto uiRandIdx = std::uniform_int_distribution<unsigned int>(0, (m_listCandidateNodes.size() - 1))(Util::g_RNG);
		for (unsigned int i = 0; i < uiRandIdx; ++i)
		{
			++it;
		}
		CSearchNode* pCandidateNode = *it;
		return PopCandidateNode(it);
	}
	// If we don't have candidate nodes then just return nullptr
	else
	{
		return nullptr;
	}
}

CSearchNode* CSearchGraph::PopCandidateNode(const std::list<CSearchNode*>::iterator& _it)
{
	CSearchNode* pCandidateNode = *_it;

	// Remove and return this candidate node from both LU table and list
	m_listCandidateNodes.erase(_it);
	TBoardPosition boardPosition = pCandidateNode->GetBoardPosition();
	LookupNode(boardPosition) = nullptr;

	// Decrement the total node count
	--m_szNodeCount;

	return pCandidateNode;
}

void CSearchGraph::PruneGraph(CSearchNode* const _kpRootNode, const std::function <bool (CSearchNode const * const)>& _fnCondition)
{
	if (_kpRootNode == nullptr)
		return;

	// Only prune if the root node passes the condition
	if (!_fnCondition || _fnCondition(_kpRootNode))
	{
		// Remove the root node from the graph
		PopNode(_kpRootNode);

		// Loop over all the different link directions
		for (unsigned int i = CSearchGraph::EDIRECTION::NORTH; i != CSearchGraph::EDIRECTION::NO_DIRECTION; ++i)
		{
			auto eLinkDirection = static_cast<CSearchGraph::EDIRECTION>(i);
			CSearchNode* pAdjNode = _kpRootNode->GetAdjHitNode(eLinkDirection);

			// Prune the graph in each direction
			PruneGraph(pAdjNode, _fnCondition);
		}

		// Delete the root node
		delete _kpRootNode;
	}
}

void CSearchGraph::PopNode(CSearchNode* const _kpPoppedNode)
{
	if (_kpPoppedNode != nullptr)
	{
		// If node is a candidate node, then simply pop it off the graph
		if (_kpPoppedNode->IsCandidateNode())
		{
			// Find candidate node in list
			for (auto it = m_listCandidateNodes.begin(); it != m_listCandidateNodes.end(); ++it)
			{
				if (_kpPoppedNode == *it)
				{
					// Pop candidate node
					PopCandidateNode(it);
					return;
				}
			}
		}

		// Shouldn't get here if the popped node is a candidate node
		assert(_kpPoppedNode->IsHitNode() && !_kpPoppedNode->IsCandidateNode());

		// For a hit node, first, manually remove it from the lookup table, and decrement the node count
		LookupNode(_kpPoppedNode->GetBoardPosition()) = nullptr;
		--m_szNodeCount;

		// Get cardinal positions
		std::array<TBoardPosition, 4> arrCardPos;
		CBoard::FillWithCardinalPositions(_kpPoppedNode->GetBoardPosition(), arrCardPos);

		//** Remove References to popped node **//
		// For adjacent cardinal positions
		for (unsigned int i = 0; i < arrCardPos.size(); ++i)
		{
			// Check cardinal position is valid
			if (!IsValidLUPosition(arrCardPos[i]))
				continue;

			// Find adjacent nodes using lookup table
			CSearchNode* pAdjNode = LookupNode(arrCardPos[i]);

			// If we find an adjacent node
			if (pAdjNode != nullptr)
			{
				// Get the link direction from the adjacent node to the popped node
				auto eDirectionToAdj = static_cast<CSearchGraph::EDIRECTION>(i);
				CSearchGraph::EDIRECTION eDirectionToPopped = CSearchGraph::GetReverseDirection(eDirectionToAdj);

				// Remove any reference to the popped node
				pAdjNode->SetAdjHitNode(eDirectionToPopped, nullptr);

				// If the adjacent node is a candidate node, with no more links
				// to any hit nodes, then it should be removed
				if (pAdjNode->IsCandidateNode() && pAdjNode->GetLinkCount() == 0)
				{
					// Pop the adjacent candidate node and delete it
					PopNode(pAdjNode);
					delete pAdjNode;
				}
			}
		}
	}
}