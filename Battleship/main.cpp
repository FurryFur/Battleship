#include <iostream>
#include <string>
#include "Board.h"
#include "BoardSquare.h"
#include "Player.h"
#include "AIPlayer.h"

typedef CBoardSquare::ESTATE ETURN_RES;

int main()
{
	CBoard player1Board;
	CBoard player2Board;

	player1Board.PlaceShipsRandom();
	player2Board.PlaceShipsRandom();

	CPlayer   player1(player1Board, player2Board);
	CAIPlayer player2(player2Board, player1Board);
	std::array< CPlayer*, 2 > arrpPlayers = { &player1, &player2 };
	
	system("cls");
	player1Board.Display(1, 1, true);
	player2Board.Display(25, 1, false);
	
	// Loop until a player wins
	while (player1.GetWinState() == CPlayer::EWIN_STATE::NO_WIN)
	{
		// For each player
		for (unsigned int i = 0; i < arrpPlayers.size(); ++i)
		{
			// Loop until it is no longer this players turn
			bool bPTurn = true;
			while (bPTurn)
			{
				// Do players turn
				bPTurn = arrpPlayers[i]->DoTurn();

				// Display the updated boards
				system("cls");
				player1Board.Display(1, 1, true);
				player2Board.Display(25, 1, false);

				// Let player know if they have another turn
				if (bPTurn)
					std::cout << "Player " << (i + 1) << " gets another turn!" << std::endl;
			}
		}
	}

	// Display the final board state with ships visible for both players
	system("cls");
	player1Board.Display(1, 1, true);
	player2Board.Display(25, 1, true);

	if (player1.GetWinState() == CPlayer::EWIN_STATE::WON)
	{
		std::cout << "You Won!" << std::endl;
	}
	else
	{
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