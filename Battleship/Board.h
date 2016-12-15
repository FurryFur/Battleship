#pragma once

#ifndef BOARD_H
#define BOARD_H

#include <random>
#include <vector>
#include "Ship.h"
#include "BoardPosition.hpp"

class CBoard
{
public:
	CBoard();
	CBoard(size_t, size_t);
	~CBoard();

	size_t GetWidth() const;
	size_t GetHeight() const;
	void DisplayAsPlayer();
	void DisplayAsOpponent() const;

	// Setup the board with random ship placement
	void SetupBoardRandom();

	// Setup the board using manual ship placement
	void SetupBoardManual();

	// Fire at a position on the board
	CBoardSquare::ESTATE FireAt(const TBoardPosition&);

	// Check that the specified position has not already been fired upon
	bool CanFireAt(const TBoardPosition&);

	// Returns a list of the ships on the board
	std::vector<CShip> GetShips() const;

	// Return a random valid board position
	TBoardPosition GetRandomBoardPosition() const;

	// Return a reference to the board square at the specified position
	CBoardSquare& GetBoardSquare(const TBoardPosition&);

private:
	std::vector<CShip> m_vecShips;
	std::vector< std::vector<CBoardSquare> > m_vec2dBoardSquares;
	
	// Check that a board position is unoccupied and in a valid range
	bool IsValidPosition(const TBoardPosition&);
	// Check ship placement is valid
	bool IsValidPlacement(const TBoardPosition&, const CShip::EORIENTATION, const CShip& _krShip);
	// Place a ship at the specified position and orientation on the board
	bool PlaceShip(const TBoardPosition&, const CShip::EORIENTATION, CShip& _rOutShip);

	const size_t m_szWidth;
	const size_t m_szHeight;

	static std::mt19937 m_skRNG;
	const std::uniform_int_distribution<unsigned int> m_kRAND_ROW;
	const std::uniform_int_distribution<unsigned int> m_kRAND_COL;
	const std::uniform_int_distribution<unsigned int> m_kRAND_ORIENTATION;

	CShip::EORIENTATION GetRandomOrientation() const;
};

#endif // BOARD_H