#pragma once

#ifndef SEARCH_GRAPH_H
#define SEARCH_GRPAH_H

#include <list>
#include <vector>

#include "BoardPosition.hpp"
#include "Board.h"

class CSearchNode;

class CSearchGraph
{
public:
	enum EDIRECTION
	{
		NORTH,
		EAST,
		SOUTH,
		WEST,
		NO_DIRECTION
	};

	static EDIRECTION GetReverseDirection(const EDIRECTION);

	CSearchGraph::CSearchGraph(const CBoard&);
	~CSearchGraph();

	// Create a new candidate node from the board position, link it to supplied adjacent node
	void AddCandidate(const TBoardPosition&, CSearchNode* const _kpAdjNode = nullptr);

	// Add a hit node to the underlying adjacency graph
	void AddHitNode(CSearchNode* const);

	// Remove and return the next most likely candidate node
	// Don't forget to delete the pointer this returns when we are done with it
	CSearchNode* PopNextCandidate();

	size_t GetCandidateCount() const;

private:
	std::list<CSearchNode*>                m_listCandidateNodes;
	std::vector<std::vector<CSearchNode*>> m_vecCandLUTable;
	size_t                                 m_szCandidateCount;

	CSearchNode* PopNode(const std::list<CSearchNode*>::iterator&);

	// Use a board position to look up an existing candidate node
	// Returns nullptr if no candidate is found at the specified position
	CSearchNode*& LookupCandidate(const TBoardPosition&);
};

#endif // SEARCH_GRAPH_H