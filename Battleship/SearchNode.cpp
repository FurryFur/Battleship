#include <math.h>
#include <algorithm>

#include "SearchNode.h"


CSearchNode::CSearchNode(const TBoardPosition& _boardPosition) :
m_boardPosition(_boardPosition),
m_arrpAdjHitNodes({ { nullptr, nullptr, nullptr, nullptr } })
{
}


CSearchNode::~CSearchNode()
{
}

void CSearchNode::SetAdjHitNode(const CSearchGraph::EDIRECTION _keDirection, CSearchNode* const _kpSearchNode)
{
	m_arrpAdjHitNodes[static_cast<unsigned int>(_keDirection)] = _kpSearchNode;
}

CSearchNode* CSearchNode::GetAdjHitNode(const CSearchGraph::EDIRECTION _keDirection)
{
	// Guard against out of bounds array indexing
	if (_keDirection == CSearchGraph::EDIRECTION::NO_DIRECTION)
		return nullptr;

	return m_arrpAdjHitNodes[static_cast<unsigned int>(_keDirection)];
}

CSearchGraph::EDIRECTION CSearchNode::GetDirectionFrom(const TBoardPosition& _krFromPos) const
{
	int iVDir = m_boardPosition.m_uiRow - _krFromPos.m_uiRow;
	int iHDir = _krFromPos.m_uiCol - m_boardPosition.m_uiCol;

	// Check the node is adjacent first
	if ((abs(iVDir) == 1 && iHDir == 0) || (abs(iHDir) == 1 && iVDir == 0))
	{
		if (iVDir != 0)
		{
			return static_cast<CSearchGraph::EDIRECTION>(iVDir + 1);
		}
		else
		{
			return static_cast<CSearchGraph::EDIRECTION>(iHDir + 2);
		}
	}
	else
	{
		return CSearchGraph::EDIRECTION::NO_DIRECTION;
	}
}

TBoardPosition CSearchNode::GetBoardPosition() const
{
	return m_boardPosition;
}