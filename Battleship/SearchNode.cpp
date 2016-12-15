#include "SearchNode.h"


CSearchNode::CSearchNode(const TBoardPosition& _boardPosition) :
m_boardPosition(_boardPosition)
{
}


CSearchNode::~CSearchNode()
{
}

void CSearchNode::SetPrevNode(CSearchNode * const _pkPrevSearchNode)
{
	m_pPrevSearchNode = _pkPrevSearchNode;
}

CSearchNode* CSearchNode::GetPrevNode() const
{
	return m_pPrevSearchNode;
}

CSearchNode* CSearchNode::GetNextNode() const
{
	return m_pNextSearchNode;
}

TBoardPosition CSearchNode::GetBoardPosition() const
{
	return m_boardPosition;
}