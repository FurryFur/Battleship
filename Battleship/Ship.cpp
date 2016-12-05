#include <assert.h>
#include "Ship.h"

CShip::CShip(size_t _szLength) :
m_szLength(_szLength)
{
	m_vecpOccupiedSquares.reserve(_szLength);
}


CShip::~CShip()
{
}

void CShip::AddOccupiedSquare(CBoardSquare const * const _kpkBoardSquare)
{
	m_vecpOccupiedSquares.push_back(_kpkBoardSquare);

	assert(m_vecpOccupiedSquares.size() <= GetLength());
}

size_t CShip::GetLength() const
{
	return m_szLength;
}