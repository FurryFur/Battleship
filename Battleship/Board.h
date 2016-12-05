#pragma once

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Ship.h"
#include "BoardPosition.hpp"

class CBoard
{
public:
	CBoard(size_t _szWidth = 10, size_t _szHeight = 10);
	~CBoard();

	bool AddShip(const BoardPosition&, const CShip::EORIENTATION, CShip& _rOutShip);
	size_t GetWidth() const;
	size_t GetHeight() const;
	void DisplayAsPlayer() const;
private:
	std::vector< std::vector<CBoardSquare> > m_vec2dSquares;
	// Check that a board position is unoccupied and in a valid range
	bool IsValidPosition(const BoardPosition&);
	// Check ship placement is valid
	bool IsValidPlacement(const BoardPosition&, const CShip::EORIENTATION, const CShip& _krShip);

	const size_t m_szWidth;
	const size_t m_szHeight;
};

#endif // BOARD_H