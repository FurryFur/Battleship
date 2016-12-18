#include <assert.h>

#include "Util.h"
#include "BoardSquare.h"
#include "Ship.h"

const std::uniform_int_distribution<unsigned int> CShip::s_kRAND_ORIENTATION(0, 1);

CShip::CShip(const CShip::ETYPE _keType) :
m_keTYPE(_keType),
m_eOrientation(CShip::EORIENTATION::NOT_PLACED),
m_bIsDestroyed(false)
{
	switch (_keType)
	{
	case CShip::PATROL_BOAT:
		m_szLength = 2;
		break;
	case CShip::SUBMARINE:
		m_szLength = 3;
		break;
	case CShip::DESTROYER:
		m_szLength = 3;
		break;
	case CShip::BATTLESHIP:
		m_szLength = 4;
		break;
	case CShip::AIRCRAFT_CARRIER:
		m_szLength = 5;
		break;
	default:
		m_szLength = 0;
		break;
	}

	m_vecpOccupiedSquares.reserve(m_szLength);
}


CShip::~CShip()
{
}

void CShip::AddOccupiedSquare(CBoardSquare * const _kpBoardSquare)
{
	m_vecpOccupiedSquares.push_back(_kpBoardSquare);

	assert(m_vecpOccupiedSquares.size() <= GetLength());
}

size_t CShip::GetLength() const
{
	return m_szLength;
}

CShip::ETYPE CShip::GetType() const
{
	return m_keTYPE;
}

CShip::EORIENTATION CShip::GetOrientation() const
{
	return m_eOrientation;
}

void CShip::SetOrientation(const CShip::EORIENTATION _keOrientation)
{
	m_eOrientation = _keOrientation;
}

CShip::EORIENTATION CShip::GetRandomOrientation()
{
	return static_cast<CShip::EORIENTATION>(s_kRAND_ORIENTATION(Util::g_RNG));
}

bool CShip::IsDestroyed() const
{
	// Short circuit if we've already checked before and come back destroyed
	if (m_bIsDestroyed)
		return true;

	// Find hit count
	unsigned int uiHits = 0;
	for (size_t i = 0; i < m_vecpOccupiedSquares.size(); ++i)
	{
		switch (m_vecpOccupiedSquares[i]->GetState())
		{
		case CBoardSquare::ESTATE::HIT:
			++uiHits;
			break;
		default:
			break;
		}
	}

	// If all ship parts have been hit
	if (uiHits == m_szLength)
	{
		// Set new internal state to destroyed
		m_bIsDestroyed = true;
	}

	return m_bIsDestroyed;
}

std::vector<CBoardSquare * const> CShip::GetOccupiedSquares() const
{
	return m_vecpOccupiedSquares;
}