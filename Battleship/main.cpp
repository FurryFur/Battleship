#include <iostream>
#include <string>
#include <regex>

#include "Util.h"
#include "Board.h"
#include "BoardSquare.h"
#include "Player.h"
#include "AIPlayer.h"

typedef CBoardSquare::ESTATE ETURN_RES;

int main()
{
	const int kiP1_VIEW_X = 2;
	const int kiP1_VIEW_Y = 0;
	const int kiP2_VIEW_X = 2;
	const int kiP2_VIEW_Y = 13;


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

	CBoard player1Board;
	CBoard player2Board;

	// Prompt the player with ship placement options
	std::cout << "Do you want to place ships manually or have them placed randomly (m/r): ";
	strInput = Util::GetValidatedInput("manual|man|m|random|rand|r");
	if (std::regex_match(strInput, std::regex("manual|man|m", std::regex_constants::icase)))
	{
		player1Board.PlaceShipsManual();
	}
	else
	{
		player1Board.PlaceShipsRandom();
	}
	
	// Computer always places ships randomly
	player2Board.PlaceShipsRandom();

	CPlayer   player1(player1Board, player2Board);
	CAIPlayer player2(player2Board, player1Board);
	std::array< CPlayer*, 2 > arrpPlayers = { &player1, &player2 };
	
	system("cls");
	player1.DisplayGameView(kiP1_VIEW_X, kiP1_VIEW_Y);
	if (bDebugView)
	{
		player2.DisplayGameView(kiP2_VIEW_X, kiP2_VIEW_Y);
	}
	
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
					Util::ClearRight();
					std::cout << "Hit! Player " << (i + 1) << " gets another turn!" << std::endl;
				}
			}
		}
	}

	// Display the final board state with ships visible for both players
	system("cls");
	player1.DisplayGameView(kiP1_VIEW_X, kiP1_VIEW_Y);
	if (bDebugView)
	{
		player2.DisplayGameView(kiP2_VIEW_X, kiP2_VIEW_Y);
	}

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

	//player1Board.FireAt({0, 1});
	
	//p1.SetupBoardManual();

	//bool bSuccess = false;
	//while (!bSuccess)
	//{
	//	TBoardPosition position;
	//	std::string strOrientation;
	//	
	//	// Get input
	//	std::cout << "Row: ";
	//	std::cin >> position.row;
	//	std::cout << "Col: ";
	//	std::cin >> position.col;
	//	std::cout << "Orientation (vert/hori): ";
	//	std::cin >> strOrientation;

	//	// TODO: Convert input to lower case
	//	
	//	CShip::EORIENTATION eOrientation;
	//	if (strOrientation == "vertical" || strOrientation == "vert")
	//	{
	//		eOrientation = CShip::EORIENTATION::VERTICAL;
	//	}
	//	else if (strOrientation == "horizontal" || strOrientation == "hori")
	//	{
	//		eOrientation = CShip::EORIENTATION::HORIZONTAL;
	//	}

	//	CShip outShip(5);
	//	bSuccess = p1.GetBoard().AddShip(position, eOrientation, outShip);
	//}

	// Display the board

	system("pause");
}