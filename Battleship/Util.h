#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <random>

#include "BoardPosition.hpp"
#include "Player.h"

namespace Util
{
	extern const std::string g_kstrREGEX_COORDS;
	extern const std::string g_kstrREGEX_ALPHA;
	extern const std::string g_kstrREGEX_NUM;

	extern std::mt19937 g_RNG;

	enum EWIN_STATE
	{
		WON,
		LOST,
		NO_WIN
	};

	// Convert the supplied string to lowercase
	std::string& ToLower(std::string&);

	// Place the console cursor and the specified (x, y) position
	void GotoXY(const int, const int);

	// Get input from console.
	// If the input doesn't match the supplied regular expression,
	// then request that the user retry entering the input
	std::string GetValidatedInput(const std::string&);

	// Converts an input string to a board position
	// If the string is not in the correct format, then the behavior
	// is undefined.
	TBoardPosition CovertInputToBoardPos(const std::string&);

	// Store the current position of the console cursor in the supplied
	// integer references
	void StoreCurCursorPos(int& _riX, int& _riY);

	// Clear everything to the right of the current console cursor position
	void ClearRight();

	// Get the win state from two players
	EWIN_STATE GetWinState(const CPlayer& _krPlayer1, const CPlayer& _krPlayer2);
}

#endif // UTIL_H