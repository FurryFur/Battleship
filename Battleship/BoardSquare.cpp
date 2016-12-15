#include "BoardSquare.h"
#include "Ship.h"


CBoardSquare::CBoardSquare() :
m_pShip(nullptr),
m_eState(CBoardSquare::ESTATE::NOT_FIRED_UPON)
{
}


CBoardSquare::~CBoardSquare()
{
}

CShip * const CBoardSquare::GetShip() const
{
	return m_pShip;
}

void CBoardSquare::SetShip(CShip * const _pShip)
{
	m_pShip = _pShip;
}

CBoardSquare::ESTATE CBoardSquare::GetState() const
{
	return m_eState;
}

void CBoardSquare::SetState(CBoardSquare::ESTATE _eState)
{
	m_eState = _eState;
}

void CBoardSquare::FireUpon()
{
	// Only update the state if it hasn't already been fired upon
	if (CanFireUpon())
	{
		// Update the board square with new state
		CShip* pkShip = GetShip();
		if (pkShip != nullptr)
		{
			SetState(CBoardSquare::ESTATE::HIT);
		}
		else
		{
			SetState(CBoardSquare::ESTATE::MISS);
		}
	}
}

bool CBoardSquare::CanFireUpon() const
{
	return (GetState() == CBoardSquare::ESTATE::NOT_FIRED_UPON);
}