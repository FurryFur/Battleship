#pragma once

#ifndef SEARCH_NODE_H
#define SEARCH_NODE_H

#include <array>

#include "BoardPosition.hpp"
#include "SearchGraph.h"

class CSearchNode
{
public:
	CSearchNode(const TBoardPosition&);
	~CSearchNode();

	void SetLink(const CSearchGraph::EDIRECTION, CSearchNode* const);
	CSearchNode* GetLink(const CSearchGraph::EDIRECTION) const;

	CSearchGraph::EDIRECTION GetDirectionFrom(const TBoardPosition&) const;

	TBoardPosition GetBoardPosition() const;

private:
	std::array<CSearchNode*, 4> m_arrpAdj;
	TBoardPosition m_boardPosition;
};

#endif // SEARCH_NODE_H