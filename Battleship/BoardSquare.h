#pragma once

#ifndef BOARD_SQUARE_H
#define BOARD_SQUARE_H

class CShip;

class CBoardSquare
{
public:
	enum EBOARDSTATE
	{
		HIT,
		MISS,
		NOT_FIRED_UPON,
		DESTROYED
	};

	CBoardSquare();
	~CBoardSquare();

	CShip const * const GetShip() const;
	void SetShip(CShip * const);
private:
	CShip*      m_pShip;
	EBOARDSTATE m_eState;
};

#endif // BOARD_SQUARE_H