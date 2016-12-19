//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name   : BoardSquare.h
// Description : Header file for CBoardSquare class
// Author      : Lance Chaney
// Mail        : lance.cha7337@mediadesign.school.nz
//

#pragma once

#ifndef BOARD_SQUARE_H
#define BOARD_SQUARE_H

class CShip;

// Holds the state at a board position
class CBoardSquare
{
public:
	enum ESTATE
	{
		HIT,
		MISS,
		NOT_FIRED_UPON,
		DESTROYED
	};

	CBoardSquare();
	CBoardSquare(const CBoardSquare&);
	~CBoardSquare();

	const CShip* GetShip() const;
	void SetShip(CShip * const);

	// Get the state of the board square (hit, miss, not fired upon, or destroyed)
	ESTATE GetState() const;

	// Set the state of the board square to a particular value
	void SetState(ESTATE);

	// Fire at this board square and update state appropriately
	void FireUpon();

	// Check that this board square has not already been fired upon
	bool CanFireUpon() const;
private:
	CShip* m_pShip;
	ESTATE m_eState;
};

#endif // BOARD_SQUARE_H