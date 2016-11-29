#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"

class Player
{
private:
	Board m_board;
public:
	Player();
	~Player();
};

#endif // PLAYER_H