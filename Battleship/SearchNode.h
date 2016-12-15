#pragma once

#ifndef SEARCH_NODE_H
#define SEARCH_NODE_H

#include "BoardPosition.hpp"

class CSearchNode
{
public:
	CSearchNode(const TBoardPosition&);
	~CSearchNode();

	void SetPrevNode(CSearchNode* const);

	CSearchNode* GetPrevNode() const;
	CSearchNode* GetNextNode() const;

	

	TBoardPosition GetBoardPosition() const;

private:
	CSearchNode*   m_pPrevSearchNode;
	CSearchNode*   m_pNextSearchNode;
	TBoardPosition m_boardPosition;
};

#endif // SEARCH_NODE_H