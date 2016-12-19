//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name   : SearchNode.cpp
// Description : Implementation file for CSearchNode class
// Author      : Lance Chaney
// Mail        : lance.cha7337@mediadesign.school.nz
//

#include <math.h>
#include <algorithm>

#include "SearchNode.h"


CSearchNode::CSearchNode(const TBoardPosition& _boardPosition) :
m_boardPosition(_boardPosition),
m_arrpAdjHitNodes({ { nullptr, nullptr, nullptr, nullptr } }),
m_bIsCandidate(true),
m_szLinkCount(0)
{
}

CSearchNode::~CSearchNode()
{
}

void CSearchNode::SetAdjHitNode(const CSearchGraph::EDIRECTION _keDirection, CSearchNode* const _kpSearchNode)
{
	// Check for link addition
	if (_kpSearchNode != nullptr && GetAdjHitNode(_keDirection) == nullptr)
	{
		++m_szLinkCount;
	}
	// Check for link removal
	if (_kpSearchNode == nullptr && GetAdjHitNode(_keDirection) != nullptr)
	{
		--m_szLinkCount;
	}

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

bool CSearchNode::IsCandidateNode() const
{
	return m_bIsCandidate;
}

void CSearchNode::SetCandidateNode()
{
	m_bIsCandidate = true;
}

bool CSearchNode::IsHitNode() const
{
	return !m_bIsCandidate;
}

void CSearchNode::SetHitNode()
{
	m_bIsCandidate = false;
}

size_t CSearchNode::GetLinkCount() const
{
	return m_szLinkCount;
}