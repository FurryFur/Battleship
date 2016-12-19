//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name   : main.cpp
// Description : Contains the main entry point for the program.
//               Contains main game loop and option selection code.
// Author      : Lance Chaney
// Mail        : lance.cha7337@mediadesign.school.nz
//

#include <iostream>
#include <string>
#include <regex>

#include "Util.h"
#include "Board.h"
#include "BoardSquare.h"
#include "Player.h"
#include "AIPlayer.h"

int main()
{
	// TF: Variable Type
	// TF: Constant
	const int kiP1_VIEW_X = 2;
	const int kiP1_VIEW_Y = 0;
	const int kiP2_VIEW_X = 2;
	const int kiP2_VIEW_Y = 13;

	bool bPlaying = true;
	while (bPlaying)
	{
		system("cls");

		// Prompt the player for debug mode select
		bool bDebugView;
		std::cout << "Do you want to play in debug mode (y/n): ";
		std::string strInput = Util::GetValidatedInput("yes|no|y|n");
		if (std::regex_match(strInput, std::regex("yes|y", std::regex_constants::icase)))
		{
			bDebugView = true;
		}
		else
		{
			bDebugView = false;
		}

		// TF: Class Instance
		CBoard player1Board;
		CBoard player2Board;

		// Prompt the player with ship placement options
		std::cout << "Do you want to place ships manually or have them placed randomly (m/r): ";
		strInput = Util::GetValidatedInput("manual|man|m|random|rand|r");
		if (std::regex_match(strInput, std::regex("manual|man|m", std::regex_constants::icase)))
		{
			// Setup board manually
			player1Board.PlaceShipsManual();
		}
		else
		{
			// Setup board with random ship placement
			player1Board.PlaceShipsRandom();
		}
		// Computer always places ships randomly
		player2Board.PlaceShipsRandom();

		// Setup players
		CPlayer   player1(player1Board, player2Board);
		CAIPlayer player2(player2Board, player1Board);
		std::array< CPlayer*, 2 > arrpPlayers = { &player1, &player2 };

		// Display initial game state
		system("cls");
		player1.DisplayGameView(kiP1_VIEW_X, kiP1_VIEW_Y);
		if (bDebugView)
		{
			player2.DisplayGameView(kiP2_VIEW_X, kiP2_VIEW_Y);
		}

		// Show whoes turn it is now
		std::cout << "Player 1's turn" << std::endl;

		// Loop until a player wins
		while (player1.GetWinState() == CPlayer::EWIN_STATE::NO_WIN)
		{
			// For each player
			for (unsigned int i = 0; i < arrpPlayers.size(); ++i)
			{
				// Loop until it is no longer this players turn
				bool bPTurn = (player1.GetWinState() == CPlayer::EWIN_STATE::NO_WIN); // No turns left if the game is over
				while (bPTurn)
				{
					// Do players turn
					bPTurn = arrpPlayers[i]->DoTurn();

					// Display the updated boards to the human player
					system("cls");
					player1.DisplayGameView(kiP1_VIEW_X, kiP1_VIEW_Y);
					if (bDebugView)
					{
						player2.DisplayGameView(kiP2_VIEW_X, kiP2_VIEW_Y);
					}

					// Let player know if they have another turn
					if (bPTurn)
					{
						std::cout << "Player " << (i + 1) << " scored a hit!" << std::endl;
						std::cout << "Player " << (i + 1) << " gets another turn!" << std::endl;
					}
					else
					{
						std::cout << "Player " << (i + 1) << " missed" << std::endl;
						std::cout << "Player " << (((i + 1) % 2) + 1) << "'s turn" << std::endl;
					}
				}
			}
		}

		// Display the final board state with ships visible for both players
		system("cls");
		player1.DisplayGameView(kiP1_VIEW_X, kiP1_VIEW_Y, true);
		if (bDebugView)
		{
			player2.DisplayGameView(kiP2_VIEW_X, kiP2_VIEW_Y, true);
		}

		// Display win/loss message
		if (player1.GetWinState() == CPlayer::EWIN_STATE::WON)
		{
			Util::ClearRight();
			std::cout << "You Won!" << std::endl;
		}
		else
		{
			Util::ClearRight();
			std::cout << "You Lost!" << std::endl;
		}

		// Ask player whether they want to play again
		std::cout << "Do you want to play again (y/n): ";
		strInput = Util::GetValidatedInput("yes|no|y|n");
		if (std::regex_match(strInput, std::regex("yes|y", std::regex_constants::icase)))
		{
			bPlaying = true;
		}
		else
		{
			bPlaying = false;
		}
	}
}