#pragma once

#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include "BoardSquare.h"

class CShip
{
public:
	enum EORIENTATION
	{
		HORIZONTAL,
		VERTICAL
	};

	CShip(size_t);
	~CShip();

	void AddOccupiedSquare(CBoardSquare const * const);
	size_t GetLength() const;
private:
	std::vector<CBoardSquare const * const> m_vecpOccupiedSquares;
	size_t m_szLength;
};

#endif // SHIP_H