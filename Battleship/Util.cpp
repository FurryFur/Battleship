#include <Windows.h>
#include <regex>
#include <iostream>
#include <string>
#include <ctype.h>
#include <algorithm>

#include "BoardPosition.hpp"

#include "Util.h"

namespace Util
{
	// Match row coordinate from A-J and column coordinate from 1-10
	const std::string g_kstrREGEX_COORDS = R"(\s*([A-Ja-j])\s*(10|[1-9])\s*|\s*(10|[1-9])\s*([A-Ja-j])\s*)";
	const std::string g_kstrREGEX_ALPHA  = R"(([A-Ja-j]))";
	const std::string g_kstrREGEX_NUM    = R"((10|[1-9]))";

	std::mt19937 g_RNG{ std::random_device()() };

	void GotoXY(const int _kiX, const int _kiY)
	{
		COORD point;
		point.X = _kiX;
		point.Y = _kiY;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
	}

	void ClearRight()
	{
		HANDLE _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(_hConsole, &csbi);

		DWORD _dwCharsToClear = csbi.dwSize.X - csbi.dwCursorPosition.X;
		DWORD _dwWritten;
		FillConsoleOutputCharacter(_hConsole, TEXT(' '), _dwCharsToClear, csbi.dwCursorPosition, &_dwWritten);
	}

	void StoreCurCursorPos(int& _riX, int& _riY)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		_riX = csbi.dwCursorPosition.X;
		_riY = csbi.dwCursorPosition.Y;
	}

	std::string& ToLower(std::string& _rStr)
	{
		std::transform(_rStr.begin(), _rStr.end(), _rStr.begin(), ::tolower);
		return _rStr;
	}

	std::string GetValidatedInput(const std::string& _krstrRegex)
	{
		std::string strInput;
		while (true)
		{
			// Save Cursor position.
			int iX, iY;
			StoreCurCursorPos(iX, iY);

			std::getline(std::cin, strInput);
			if (std::regex_match(strInput, std::regex(R"(\s*)" + _krstrRegex + R"(\s*)", std::regex_constants::icase)))
				return strInput;
			else
			{
				// Print error at message position
				Util::ClearRight(); // Prevent overlapping messages
				std::cout << "Invalid Input";

				// Reset cursor position
				GotoXY(iX, iY);

				// Clear previous input
				ClearRight();
			}
		}
	}

	TBoardPosition CovertInputToBoardPos(const std::string& _krstrInput)
	{
		std::smatch smLetter;
		std::smatch smNumber;
		std::regex_search(_krstrInput, smLetter, std::regex(g_kstrREGEX_ALPHA, std::regex_constants::icase));
		std::regex_search(_krstrInput, smNumber, std::regex(g_kstrREGEX_NUM, std::regex_constants::icase));
		std::string strLetter = smLetter[0];

		std::string strNumber = smNumber[0];
		char cLetter = tolower(strLetter[0]);

		auto uiRow = static_cast<unsigned int>(strLetter[0] - 'a');
		auto uiCol = static_cast<unsigned int>(std::stoul(strNumber) - 1);

		return{ uiRow, uiCol };
	}
}