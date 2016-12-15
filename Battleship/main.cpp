#include <iostream>
#include <string>
#include "Board.h"
#include "Player.h"
#include "AIPlayer.h"

int main()
{
	CBoard player1Board;
	CBoard player2Board;

	// Create board with randomly placed ships
	player1Board.SetupBoardRandom();
	player2Board.SetupBoardRandom();

	CPlayer   player1{ player1Board, player2Board };
	CAIPlayer player2{ player2Board, player1Board };

	while (player1.GetWinState() == CPlayer::EWIN_STATE::NO_WIN)
	{
		player1.DoTurn();
		player2.DoTurn();

		player1Board.DisplayAsPlayer();
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