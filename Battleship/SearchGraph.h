#pragma once

#ifndef SEARCH_GRAPH_H
#define SEARCH_GRPAH_H

#include <list>
#include <vector>
#include <functional>

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

	// Create a new candidate node from the board position, link it to the supplied adjacent node
	void AddCandidate(const TBoardPosition&, CSearchNode* const _kpAdjNode = nullptr);

	// Add a hit node to the underlying adjacency graph
	void AddHitNode(CSearchNode* const);

	// Remove and return the next most likely candidate node
	// Don't forget to delete the pointer this returns when we are done with it
	CSearchNode* PopNextCandidate();

	// Walk the graph in all direction from the specified root node pruneing all nodes along the way.
	// If the condition function is specified, then only nodes passing the condition will be pruned
	void CSearchGraph::PruneGraph(CSearchNode* const, const std::function <bool(CSearchNode const * const)>& fnCondition);

	size_t GetCandidateCount() const;
	size_t GetNodeCount() const;

private:
	std::list<CSearchNode*>                m_listCandidateNodes;
	std::vector<std::vector<CSearchNode*>> m_vecNodeLUTable;
	size_t                                 m_szNodeCount;
	size_t                                 m_szLUTWidth;
	size_t                                 m_szLUTHeight;

	// Remove and return a candidate node at the specified iterator position
	CSearchNode* PopCandidateNode(const std::list<CSearchNode*>::iterator&);

	bool IsValidLUPosition(const TBoardPosition& _krPosition) const;

	// Use a board position to look up an existing candidate node
	// Returns nullptr if no candidate is found at the specified position
	CSearchNode*& LookupNode(const TBoardPosition&);

	// Remove the specified node and any links in the graph to said node.
	// If another node ends up disconnected from the graph due to this pop,
	// that node is poped AND deleted. This function does not affect the state
	// of the poped node.
	void PopNode(CSearchNode* const);
};

#endif // SEARCH_GRAPH_H