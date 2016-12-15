#include "SearchGraph.h"


CSearchGraph::CSearchGraph() :
m_pHead(nullptr),
m_szSize(0)
{
}


CSearchGraph::~CSearchGraph()
{
	while (m_pHead != nullptr)
	{
		CSearchNode* pTmp = m_pHead;
		m_pHead = m_pHead->GetPrevNode();
		delete pTmp;
	}
}

void CSearchGraph::AddSearchPosition(const TBoardPosition& _krBoardPosition)
{
	CSearchNode* pSearchNode = new CSearchNode(_krBoardPosition);
	if (pSearchNode != nullptr)
	{
		pSearchNode->SetPrevNode(m_pHead);
		m_pHead = pSearchNode;
	}

	++m_szSize;
}

size_t CSearchGraph::GetSize() const
{
	return m_szSize;
}