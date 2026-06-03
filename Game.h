#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector>

class Game
{
	Ball ball;
	Box paddle;

	// TODO #1 - Instead of storing 1 brick, store a vector of bricks (by value)
	//Lol, simply copy paste error on my part, no idea why I had created an array of 5 vectors, but fixed it now.
	//Thought it was weird to have my "." changed to arrows, but it was working so I didn't bother to troubleshoot until now.
	std::vector<Box> bricks;

	//Ok, after reviewing the instructions on FSO before submitting, I see that I didn't do everything correctly.
	//I needed someway to render the text in the Render method that doesn't have any parameters so rather than trying to add a
	//	parameter that might break the whole game, I added these two booleans to tell the game if it should continue rendering or
	//	simply display the end screen.
	bool end = false;
	bool victory = false;

public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};