#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Board.h"
#include "Ship.h"

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	CBoard& GetBoard();

private:
	CBoard              m_board;
	std::vector<CShip>  m_vShips;
};

#endif // PLAYER_H