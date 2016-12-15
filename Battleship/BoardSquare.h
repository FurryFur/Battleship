#pragma once

#ifndef BOARD_SQUARE_H
#define BOARD_SQUARE_H

class CShip;

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
	~CBoardSquare();

	CShip * const GetShip() const;
	void SetShip(CShip * const);

	// Get the state of the board (hit, miss, not fired upon or destroyed)
	ESTATE GetState() const;

	// Set the state of the board square to a particular value
	void SetState(ESTATE);

	// Update the state of the board square to the appropriate state after being fired upon
	void FireUpon();

	// Check that this board square has not already been fired upon
	bool CanFireUpon() const;
private:
	CShip* m_pShip;
	ESTATE m_eState;
};

#endif // BOARD_SQUARE_H