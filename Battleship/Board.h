#pragma once

#ifndef BOARD_H
#define BOARD_H

#include <random>
#include <vector>
#include <array>

#include "Ship.h"
#include "BoardPosition.hpp"

class CBoard
{
public:
	CBoard();
	CBoard(size_t, size_t);
	CBoard(const CBoard&);
	~CBoard();

	size_t GetWidth() const;
	size_t GetHeight() const;

	// Fills the supplied array with board positions, NORTH, SOUTH, EAST, and 
	// WEST, of the specified position
	static std::array<TBoardPosition, 4>& FillWithCardinalPositions(const TBoardPosition&, std::array<TBoardPosition, 4>&);

	// Display the board and pos (x, y), with ships visible or invisible
	void Display(const int _kiX, const int _kiY, const bool _kbShipsVisible) const;

	// Fire at a position on the board
	CBoardSquare::ESTATE FireAt(const TBoardPosition&);

	// Check that the specified position has not already been fired upon
	bool CanFireAt(const TBoardPosition&) const;

	// Return a random board position on this board
	TBoardPosition GetRandomBoardPosition() const;

	// Return a pointer to the board square (const) at the specified position
	const CBoardSquare& GetBoardSquare(const TBoardPosition&) const;

	// Try placing a ship at the specified position and orientation on the board.
	// Return true on success, and false on failure.
	bool TryPlaceShip(const TBoardPosition&, const CShip::EORIENTATION, CShip& _rOutShip);

	// Place all ships randomly on the board
	void PlaceShipsRandom();

	// Place all ships using user input from the player
	void PlaceShipsManual();

	// Returns a list of the ships on the board
	const CShip& GetShip(const unsigned int) const;

	// Returns the number of available ships (may or may not be placed)
	size_t GetShipCount() const;

private:
	std::vector< std::vector<CBoardSquare> > m_vec2dBoardSquares;
	
	// Check that a board position is inside the board
	bool IsValidPosition(const TBoardPosition&) const;
	// Check ship placement is valid
	bool IsValidPlacement(const TBoardPosition&, const CShip::EORIENTATION, const CShip&) const;

	// Return a modifiable reference to the board square at the specified position
	CBoardSquare& _GetBoardSquare(const TBoardPosition&);

	std::vector<CShip> m_vecShips;

	const size_t m_szWidth;
	const size_t m_szHeight;

	const std::uniform_int_distribution<unsigned int> m_kRAND_ROW;
	const std::uniform_int_distribution<unsigned int> m_kRAND_COL;
};

#endif // BOARD_H