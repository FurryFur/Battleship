Debug Mode:
	The game will initially ask the player if they want to play the game in debug mode.
	This mode shows the the board states from both the player and computers point of view.
	Valid responses for the debug mode option are: yes, y, no, n

Manual/Random Ship Placement:
	Players will be prompted on whether they want to place ships manually or have the 
	computer do it randomly for them.
	Valid responses for this option are: manual, man, m, random, rand, r

	If a player decides to place their ships manually then the game will ask for coordinates
	for each ship to place.
	Coordinates should be input in the following form: <Coorindate Letter><Coordinate Number> 
	                                                or <Coordinate Number><Coordinate Letter>
	Eg: A5

	The player will then be asked what orientation they want to place the ship in.
	Valid inputs are: horizontal, horiz, hori, hor, h, vertical, vert, v
	For the horizontal orientation: Ships parts are placed, starting at the input coordinate,
	                                and going east.
	For the vertical orientation:   Ship parts are placed, starting at the input coordinate,
	                                and going south.
	
Gameplay
	On the playes turn, they will be prompted to enter a coordinate on the oppenents board
	to fire upon.
	Coordinates should be input in the following form: <Coorindate Letter><Coordinate Number> 
	                                                or <Coordinate Number><Coordinate Letter>
	Eg: 6b

NOTE: All inputs are case insensitive (i.e. A5 = a5)