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

	// Set the the adjacent node in the given direction to be the specified node
	void SetAdjNode(const CSearchGraph::EDIRECTION, CSearchNode* const);

	// Get a pointer the adjacent node in the specified direction
	CSearchNode* GetAdjNode(const CSearchGraph::EDIRECTION) const;

	// Returns the direction to this node from the specified ADJACENT board position
	// If the board position is not adjacent then NO_DIRECTION is returned
	CSearchGraph::EDIRECTION GetDirectionFrom(const TBoardPosition&) const;

	TBoardPosition GetBoardPosition() const;

private:
	// Array of adjacent nodes, indexed by direction (NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3)
	std::array<CSearchNode*, 4> m_arrpAdj;

	TBoardPosition m_boardPosition;
};

#endif // SEARCH_NODE_H