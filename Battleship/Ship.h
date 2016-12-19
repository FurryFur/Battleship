//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name   : Ship.h
// Description : Header file for CShip class
// Author      : Lance Chaney
// Mail        : lance.cha7337@mediadesign.school.nz
//

#pragma once

#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <random>
#include <string>

#include "BoardSquare.h"

// Represents a ship in the game.
class CShip
{
public:
	enum EORIENTATION
	{
		HORIZONTAL,
		VERTICAL,
		NOT_PLACED
	};

	enum ETYPE
	{
		PATROL_BOAT,
		SUBMARINE,
		DESTROYER,
		BATTLESHIP,
		AIRCRAFT_CARRIER
	};

	CShip(ETYPE);
	CShip(const CShip&);
	~CShip();

	// Adds a board square to the ships list of occupied squares
	void AddOccupiedSquare(CBoardSquare * const);

	// Returns the length of the ship (how many board square it occupies)
	size_t GetLength() const;

	// Returns the type of the ship (eg: PATROL_BOAT)
	ETYPE GetType() const;
	
	// Returns the orientation that this ship has been placed in
	// If the ship has not been placed then this method returns 
	// EORIENTATION::NOT_PLACED
	EORIENTATION GetOrientation() const;
	
	// Sets the ships orientation
	void SetOrientation(EORIENTATION);

	// Checks if all the squares occupied by the ship have been hit
	bool IsDestroyed() const;

	// Returns the vector of board square pointers occupied by the ship
	std::vector<CBoardSquare * const> GetOccupiedSquares() const;

	// Returns the name for this type of ship
	std::string GetName() const;

	// Returns a random ship orientation
	static CShip::EORIENTATION GetRandomOrientation();

private:
	std::vector<CBoardSquare * const> m_vecpOccupiedSquares;
	size_t m_szLength;
	mutable bool m_bIsDestroyed; // Cache variable
	EORIENTATION m_eOrientation;
	const ETYPE m_keTYPE;

	static const std::uniform_int_distribution<unsigned int> s_kRAND_ORIENTATION;
};

#endif // SHIP_H