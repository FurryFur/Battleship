#include <iostream>
#include <random>
#include <Windows.h>
#include <vector>

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

bool CBoard::IsValidPosition(const TBoardPosition& _krPosition)
{
	// Check if position is on the board
	if (_krPosition.m_uiRow < GetHeight() && _krPosition.m_uiCol < GetWidth())
	{
		return true;
	}

	return false;
}

bool CBoard::IsValidPlacement(const TBoardPosition& _krPosition, const CShip::EORIENTATION _keOrientation, const CShip& _krOutShip)
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
		GetBoardSquare({ r, c }).SetShip(&_rOutShip);
		_rOutShip.AddOccupiedSquare(&GetBoardSquare({ r, c }));

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

void CBoard::DisplayAsPlayer()
{
	system("cls");

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

			// Check for ship
			const CShip* pkShip = krBoardSquare.GetShip();
			auto cChar = static_cast<unsigned char>(247);
			WORD wColor = kwGRID_DEFAULT_C;
			if (pkShip != nullptr)
			{
				// Set coloring and chracter for ship
				cChar = (krBoardSquare.GetState() == CBoardSquare::ESTATE::NOT_FIRED_UPON) ? '=' : 'X';
				wColor = ((static_cast<WORD>(pkShip->GetType()) + 2) * 0x10);
				if (krBoardSquare.GetState() == CBoardSquare::ESTATE::HIT)
				{
					wColor = wColor | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				}
				else if (krBoardSquare.GetState() == CBoardSquare::ESTATE::DESTROYED)
				{
					wColor = ((wColor | FOREGROUND_RED) & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE) | FOREGROUND_INTENSITY
					       | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
				}
			}

			// Add spacing between grid squares
			if (c != 0)
			{
				// Set coloring for spacing
				const CBoardSquare& krBoardSquareLeft = GetBoardSquare({ r, c - 1 });
				if (krBoardSquareLeft.GetShip() != pkShip)
				{
					SetConsoleTextAttribute(hConsole, kwGRID_DEFAULT_C);
				}
				else
				{
					if (krBoardSquare.GetState() == CBoardSquare::ESTATE::MISS && krBoardSquareLeft.GetState() == CBoardSquare::ESTATE::MISS)
					{
						SetConsoleTextAttribute(hConsole, wColor & ~BACKGROUND_BLUE & ~BACKGROUND_RED & ~BACKGROUND_GREEN);
					}
					else
					{
						SetConsoleTextAttribute(hConsole, wColor);
					}
				}

				// Print spacing
				std::cout << ' ';
			}

			// Set coloring for grid square
			if (krBoardSquare.GetState() == CBoardSquare::ESTATE::MISS)
			{
				SetConsoleTextAttribute(hConsole, wColor & ~BACKGROUND_BLUE & ~BACKGROUND_RED & ~BACKGROUND_GREEN);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, wColor);
			}

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

void CBoard::DisplayAsOpponent() const
{

}

CBoardSquare::ESTATE CBoard::FireAt(const TBoardPosition& _krBoardPos)
{
	CBoardSquare& rBoardSquare = GetBoardSquare(_krBoardPos);
	rBoardSquare.FireUpon();

	// Check if we have destroyed a ship and update board state appropriately
	CShip* pkShip = rBoardSquare.GetShip();
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

bool CBoard::CanFireAt(const TBoardPosition& _krBoardPos)
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

std::vector<CShip> CBoard::GetShips() const
{
	return m_vecShips;
}

TBoardPosition CBoard::GetRandomBoardPosition() const
{
	return TBoardPosition{ m_kRAND_ROW(m_skRNG), m_kRAND_COL(m_skRNG) };
}

CShip::EORIENTATION CBoard::GetRandomOrientation() const
{
	return static_cast<CShip::EORIENTATION>(m_kRAND_ORIENTATION(m_skRNG));
}

CBoardSquare& CBoard::GetBoardSquare(const TBoardPosition& _krBoardPosition)
{
	return m_vec2dBoardSquares[_krBoardPosition.m_uiRow][_krBoardPosition.m_uiCol];
}