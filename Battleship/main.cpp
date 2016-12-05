#include <iostream>
#include <string>
#include "Player.h"

int main()
{
	// Create board with a single randomly placed ship
	CPlayer p1;

	p1.GetBoard().DisplayAsPlayer();

	bool bSuccess = false;
	while (!bSuccess)
	{
		BoardPosition position;
		std::string strOrientation;
		
		// Get input
		std::cout << "Row: ";
		std::cin >> position.row;
		std::cout << "Col: ";
		std::cin >> position.col;
		std::cout << "Orientation (vert/hori): ";
		std::cin >> strOrientation;

		// TODO: Convert input to lower case
		
		CShip::EORIENTATION eOrientation;
		if (strOrientation == "vertical" || strOrientation == "vert")
		{
			eOrientation = CShip::EORIENTATION::VERTICAL;
		}
		else if (strOrientation == "horizontal" || strOrientation == "hori")
		{
			eOrientation = CShip::EORIENTATION::HORIZONTAL;
		}

		CShip outShip(5);
		bSuccess = p1.GetBoard().AddShip(position, eOrientation, outShip);
	}

	// Display the board
	p1.GetBoard().DisplayAsPlayer();
	//p1.GetBoard().DisplayAsOpponent();

	system("pause");
}