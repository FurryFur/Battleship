#include <iostream>
#include "Board.h"
#include "BoardSquare.h"
#include "BoardPosition.hpp"
#include "Ship.h"

CBoard::CBoard(size_t _szWidth, size_t _szHeight) : 
m_szWidth(_szWidth),
m_szHeight(_szHeight),
m_vec2dSquares(_szHeight, std::vector<CBoardSquare>(_szWidth))
{
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

bool CBoard::IsValidPosition(const BoardPosition& _krPosition)
{
	// Check if its on the board
	if (_krPosition.row < GetHeight() && _krPosition.col < GetWidth())
	{
		// Check if its unoccupied
		if (m_vec2dSquares[_krPosition.row][_krPosition.col].GetShip() == nullptr)
		{
			return true;
		}
	}

	return false;
}

bool CBoard::IsValidPlacement(const BoardPosition& _krPosition, const CShip::EORIENTATION _keOrientation, const CShip& _krOutShip)
{
	unsigned int r = _krPosition.row;
	unsigned int c = _krPosition.col;
	// Iterate through the board positions that the ship will occupy
	for (unsigned int i = 0; i < _krOutShip.GetLength(); ++i)
	{
		// Fail if one of the positions is not valid or out of range
		if (!IsValidPosition({ r, c }))
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


bool CBoard::AddShip(const BoardPosition& _krPosition, const CShip::EORIENTATION _keOrientation, CShip& _rOutShip)
{
	if (!IsValidPlacement(_krPosition, _keOrientation, _rOutShip))
	{
		return false;
	}

	unsigned int r = _krPosition.row;
	unsigned int c = _krPosition.col;
	for (unsigned int i = 0; i < _rOutShip.GetLength(); ++i)
	{
		m_vec2dSquares[r][c].SetShip(&_rOutShip);
		_rOutShip.AddOccupiedSquare(&m_vec2dSquares[r][c]);

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

void CBoard::DisplayAsPlayer() const
{
	for (unsigned int r = 0; r < GetHeight(); ++r)
	{
		for (unsigned int c = 0; c < GetWidth(); ++c)
		{
			char cChar = m_vec2dSquares[r][c].GetShip() == nullptr ? ' ' : 'X';
			std::cout << "|" << cChar;
		}
		std::cout << "|" << std::endl;
	}
}