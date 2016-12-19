//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name   : SeachNode.h
// Description : Header file for CSearchNode class
// Author      : Lance Chaney
// Mail        : lance.cha7337@mediadesign.school.nz
//

#pragma once

#ifndef SEARCH_NODE_H
#define SEARCH_NODE_H

#include <array>

#include "BoardPosition.hpp"
#include "SearchGraph.h"

// Represents a node in the AI search graph
class CSearchNode
{
public:
	CSearchNode(const TBoardPosition&);
	~CSearchNode();

	// Set the the adjacent node in the given direction to be the specified node
	void SetAdjHitNode(const CSearchGraph::EDIRECTION, CSearchNode* const);

	// Get a pointer the adjacent node in the specified direction
	CSearchNode* GetAdjHitNode(const CSearchGraph::EDIRECTION);

	// Returns the direction to this node from the specified ADJACENT board position
	// If the board position is not adjacent then NO_DIRECTION is returned
	CSearchGraph::EDIRECTION GetDirectionFrom(const TBoardPosition&) const;

	// Returns the board position associated with this node
	TBoardPosition GetBoardPosition() const;

	// Check if this node is a candidate node
	bool IsCandidateNode() const;

	// Set the node as a candidate node
	void SetCandidateNode();

	// Check if this node is a hit node
	bool IsHitNode() const;

	// Set the node as a hit node
	void SetHitNode();

	// Returns the number of links from this node to other nodes in the graph
	size_t GetLinkCount() const;

private:
	// Array of adjacent nodes, indexed by direction (NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3)
	std::array<CSearchNode*, 4> m_arrpAdjHitNodes;

	TBoardPosition m_boardPosition;
	bool m_bIsCandidate;
	size_t m_szLinkCount;
};

#endif // SEARCH_NODE_H