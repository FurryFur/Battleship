#pragma once

#ifndef SEARCH_GRAPH_H
#define SEARCH_GRPAH_H

#include "SearchNode.h"

class CSearchGraph
{
public:
	CSearchGraph();
	~CSearchGraph();

	void AddSearchPosition(const TBoardPosition&);
	CSearchNode* GetBestNode();

	size_t GetSize() const;

private:
	CSearchNode* m_pHead;
	CSearchNode* m_pTail;
	size_t       m_szSize;
};

#endif // SEARCH_GRAPH_H