#include <iostream>
#include <random>
#include <Windows.h>
#include <vector>
#include <array>

#include "BoardSquare.h"
#include "BoardPosition.hpp"
#include "Ship.h"

#include "Board.h"

std::mt19937 CBoard::m_skRNG{ std::random_device()() };

CBoard::CBoard() :
CBoard::CBoard(10, 10)
{
}

CBoard::CBoard(size_t _szWidth, size_t _szHeight) : 
m_szWidth(_szWidth),
m_szHeight(_szHeight),
m_vec2dBoardSquares(_szHeight, std::vector<CBoardSquare>(_szWidth)),
m_kRAND_ROW(0, _szHeight - 1),
m_kRAND_COL(0, _szWidth - 1),
m_kRAND_ORIENTATION(0, 1)
{
	m_vecShips.emplace_back(CShip::ETYPE::PATROL_BOAT);
	m_vecShips.emplace_back(CShip::ETYPE::SUBMARINE);
	m_vecShips.emplace_back(CShip::ETYPE::DESTROYER);
	m_vecShips.emplace_back(CShip::ETYPE::BATTLESHIP);
	m_vecShips.emplace_back(CShip::ETYPE::AIRCRAFT_CARRIER);
}


CBoard::~CBoard()
{
}

size_t CBoard::GetWidth() const
{
	return m_szWidth;
}

size_t CBoard::GetHeight() const
{
	return m_szHeight;
}

std::array<TBoardPosition, 4>& CBoard::FillWithCardinalPositions(const TBoardPosition& _kBoardPos, std::array<TBoardPosition, 4>& _rarrCardPos)
{
	_rarrCardPos[0].m_uiRow = _kBoardPos.m_uiRow - 1; _rarrCardPos[0].m_uiCol = _kBoardPos.m_uiCol;
	_rarrCardPos[1].m_uiRow = _kBoardPos.m_uiRow;     _rarrCardPos[1].m_uiCol = _kBoardPos.m_uiCol + 1;
	_rarrCardPos[2].m_uiRow = _kBoardPos.m_uiRow + 1; _rarrCardPos[2].m_uiCol = _kBoardPos.m_uiCol;
	_rarrCardPos[3].m_uiRow = _kBoardPos.m_uiRow;     _rarrCardPos[3].m_uiCol = _kBoardPos.m_uiCol - 1;
	return _rarrCardPos;
}

bool CBoard::IsValidPosition(const TBoardPosition& _krPosition) const
{
	// Check if position is on the board
	if (_krPosition.m_uiRow < GetHeight() && _krPosition.m_uiCol < GetWidth())
	{
		return true;
	}

	return false;
}

bool CBoard::IsValidPlacement(const TBoardPosition& _krPosition, const CShip::EORIENTATION _keOrientation, const CShip& _krOutShip) const
{
	unsigned int r = _krPosition.m_uiRow;
	unsigned int c = _krPosition.m_uiCol;
	// Iterate through the board positions that the ship will occupy
	for (unsigned int i = 0; i < _krOutShip.GetLength(); ++i)
	{
		// Fail if one of the positions is not valid or if it's already occupied
		if (!IsValidPosition({ r, c }) || GetBoardSquare({ r, c }).GetShip() != nullptr)
		{
			return false;
		}

		switch (_keOrientation)
		{
		case CShip::EORIENTATION::VERTICAL:
			++r;
			break;
		case CShip::EORIENTATION::HORIZONTAL:
		default:
			++c;
			break;
		}
	}

	return true;
}


bool CBoard::PlaceShip(const TBoardPosition& _krPosition, const CShip::EORIENTATION _keOrientation, CShip& _rOutShip)
{
	if (!IsValidPlacement(_krPosition, _keOrientation, _rOutShip))
	{
		return false;
	}

	// Set the ships orientation now that it's been placed on the board
	_rOutShip.SetOrientation(_keOrientation);

	unsigned int r = _krPosition.m_uiRow;
	unsigned int c = _krPosition.m_uiCol;
	for (unsigned int i = 0; i < _rOutShip.GetLength(); ++i)
	{
		_GetBoardSquare({ r, c }).SetShip(&_rOutShip);
		_rOutShip.AddOccupiedSquare(&_GetBoardSquare({ r, c }));

		switch (_keOrientation)
		{
		case CShip::EORIENTATION::VERTICAL:
			++r;
			break;
		case CShip::EORIENTATION::HORIZONTAL:
		default:
			++c;
			break;
		}
	}
	
	
	return true;
}

void CBoard::SetupBoardRandom()
{
	for (unsigned int i = 0; i < m_vecShips.size(); ++i)
	{
		bool bSuccess = false;
		while (!bSuccess)
		{
			unsigned int uiR = m_kRAND_ROW(m_skRNG);
			unsigned int uiC = m_kRAND_COL(m_skRNG);

			bSuccess = PlaceShip(
				GetRandomBoardPosition(),
				GetRandomOrientation(), 
				m_vecShips[i]
			);
		}
	}
}

void CBoard::Display(const bool _bShipsVisible) const
{
	typedef CBoardSquare::ESTATE EHIT_STATE;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	const WORD kwGRID_DEFAULT_C = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	// Set grid letters coloring
	SetConsoleTextAttribute(hConsole, csbi.wAttributes);

	// Print grid numbers
	std::cout << "   ";
	for (unsigned int c = 0; c < GetWidth(); ++c)
	{
		std::cout << c + 1 << ' ';
	}
	std::cout << std::endl;

	for (unsigned int r = 0; r < GetHeight(); ++r)
	{
		// Set grid numbers coloring
		SetConsoleTextAttribute(hConsole, csbi.wAttributes);

		// Print grid letters
		std::cout << ' ' << static_cast<char>('A' + r) << ' ';

		// Set grid default coloring
		SetConsoleTextAttribute(hConsole, kwGRID_DEFAULT_C);

		for (unsigned int c = 0; c < GetWidth(); ++c)
		{
			const CBoardSquare& krBoardSquare = GetBoardSquare({ r, c });

			auto cChar = static_cast<unsigned char>(247);
			WORD wColor = kwGRID_DEFAULT_C;
			WORD wSpacingColor = kwGRID_DEFAULT_C;

			// If ships are visible
			if (_bShipsVisible)
			{
				// Check for ship
				const CShip* pkShip = krBoardSquare.GetShip();
			
				if (pkShip != nullptr)
				{
					// Set coloring and chracter for ship square
					cChar = (krBoardSquare.GetState() == EHIT_STATE::NOT_FIRED_UPON) ? '=' : 'X';
					wColor = ((static_cast<WORD>(pkShip->GetType()) + 2) * 0x10);
				}
			}

			// Set character and character coloring for hit squares
			if (krBoardSquare.GetState() == CBoardSquare::ESTATE::HIT)
			{
				cChar = 'X';
				wColor = wColor | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			}
			// Set character and character coloring for destroyed squares
			if (krBoardSquare.GetState() == CBoardSquare::ESTATE::DESTROYED)
			{
				cChar = 'X';
				wColor = ((wColor | FOREGROUND_RED) & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE) | FOREGROUND_INTENSITY
					| BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
			}

			// Add spacing between grid squares
			if (c != 0)
			{
				// Set coloring for spacing
				const CBoardSquare& krBoardSquareLeft = GetBoardSquare({ r, c - 1 });
				EHIT_STATE eLeftState = krBoardSquareLeft.GetState();
				EHIT_STATE eRightState = krBoardSquare.GetState();
				// Black background between misses, miss and destroyed, and miss and hit
				bool bBothMisses  = (eLeftState == EHIT_STATE::MISS && eRightState == EHIT_STATE::MISS);
				bool bMissHit     = (eLeftState == EHIT_STATE::MISS && eRightState == EHIT_STATE::HIT) 
				                 || (eLeftState == EHIT_STATE::HIT && eRightState == EHIT_STATE::MISS);
				bool bMissDestroy = (eLeftState == EHIT_STATE::MISS && eRightState == EHIT_STATE::DESTROYED)
				                 || (eLeftState == EHIT_STATE::DESTROYED && eRightState == EHIT_STATE::MISS);
				if (bBothMisses || bMissHit || bMissDestroy)
				{
					wSpacingColor &= ~BACKGROUND_BLUE & ~BACKGROUND_RED & ~BACKGROUND_GREEN;
				}
				// White background between destroyed
				else if (eLeftState == EHIT_STATE::DESTROYED && eRightState == EHIT_STATE::DESTROYED)
				{
					wSpacingColor |= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
				}
				// Ship color background between ships if ships are visible
				else if (_bShipsVisible && krBoardSquare.GetShip() != nullptr && krBoardSquareLeft.GetShip() == krBoardSquare.GetShip())
				{
					wSpacingColor = wColor;
				}
				// Blue unfired upon background by default
				else
				{
					wSpacingColor = kwGRID_DEFAULT_C;
				}

				SetConsoleTextAttribute(hConsole, wSpacingColor);

				// Print spacing
				std::cout << ' ';
			}

			// Set coloring for grid square
			if (krBoardSquare.GetState() == EHIT_STATE::MISS)
			{
				// Black background on misses
				cChar = 'O';
				wColor = ~BACKGROUND_BLUE & ~BACKGROUND_RED & ~BACKGROUND_GREEN;
				wColor = FOREGROUND_RED | FOREGROUND_BLUE;
			}
			SetConsoleTextAttribute(hConsole, wColor);

			// Print grid square
			std::cout << cChar;
		}

		// End row
		std::cout << std::endl;
	}

	// Reset coloring
	SetConsoleTextAttribute(hConsole, csbi.wAttributes);

	// Add margin to bottom
	std::cout << std::endl << std::endl;
}

CBoardSquare::ESTATE CBoard::FireAt(const TBoardPosition& _krBoardPos)
{
	CBoardSquare& rBoardSquare = _GetBoardSquare(_krBoardPos);
	rBoardSquare.FireUpon();

	// Check if we have destroyed a ship and update board state appropriately
	const CShip* pkShip = rBoardSquare.GetShip();
	if (pkShip != nullptr && pkShip->IsDestroyed())
	{
		// Update board squares to reflect new destroyed ship state
		std::vector<CBoardSquare* const> vecShipSquares = pkShip->GetOccupiedSquares();
		for (size_t i = 0; i < vecShipSquares.size(); ++i)
		{
			vecShipSquares[i]->SetState(CBoardSquare::ESTATE::DESTROYED);
		}
	}

	return rBoardSquare.GetState();
}

bool CBoard::CanFireAt(const TBoardPosition& _krBoardPos) const
{
	// Check index in range
	if (IsValidPosition(_krBoardPos))
	{
		return GetBoardSquare(_krBoardPos).CanFireUpon();
	}
	else
	{
		return false;
	}
}

const CShip& CBoard::GetShip(const unsigned int _kuiIdx) const
{
	return m_vecShips[_kuiIdx];
}

size_t CBoard::GetShipCount() const
{
	return m_vecShips.size();
}

TBoardPosition CBoard::GetRandomBoardPosition() const
{
	return TBoardPosition{ m_kRAND_ROW(m_skRNG), m_kRAND_COL(m_skRNG) };
}

CShip::EORIENTATION CBoard::GetRandomOrientation() const
{
	return static_cast<CShip::EORIENTATION>(m_kRAND_ORIENTATION(m_skRNG));
}

const CBoardSquare& CBoard::GetBoardSquare(const TBoardPosition& _krBoardPosition) const
{
	return m_vec2dBoardSquares[_krBoardPosition.m_uiRow][_krBoardPosition.m_uiCol];
}

CBoardSquare& CBoard::_GetBoardSquare(const TBoardPosition& _krBoardPosition)
{
	return m_vec2dBoardSquares[_krBoardPosition.m_uiRow][_krBoardPosition.m_uiCol];
}