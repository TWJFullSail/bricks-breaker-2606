#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	end = false;
	bricks.clear();
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	Box brick;
	brick.width = 10;
	brick.height = 2;
	brick.x_position = 0;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkCyan; //Changed to Dark Cyan so that Black would be 3 hits, not 2.
	
	for (size_t i = 0; i < 5; ++i) {
		bricks.push_back(brick);
		//Not sure if this is necessary for the lab, but without this the boxes would all be drawn over each other.
		//Nvm, this is in the instructions on FSO to evenly space them.
		int spaceBetween = ((WINDOW_WIDTH - brick.width) - (brick.width*4)) / 4;
		brick.x_position += brick.width + spaceBetween;
	};
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	if (!end) {
		paddle.Draw();
		ball.Draw();

		// TODO #3 - Update render to render all bricks
		for (std::vector<Box>::const_iterator i = bricks.begin(); i < bricks.end(); ++i) {
			i->Draw();
		};		
	}
	else {
		Console::SetCursorPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); 		
		
		if (victory) {
			std::cout << " You win! \n ";
		}
		else {
			std::cout << " You lose. \n ";
		};

		Console::SetCursorPosition(WINDOW_WIDTH / 2, Console::CursorRow());
		std::cout << "Press 'R' to play again.";
	};

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (std::vector<Box>::iterator i = bricks.begin(); i < bricks.end();) {
		if (i->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			i->color = ConsoleColor(i->color - 1);
			ball.y_velocity *= -1;			
		};

		// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
		if (ConsoleColor::Black == i->color) {
			i = bricks.erase(i);			
		}
		else {
			++i;
		};
	};
	

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.size() < 1) {
		ball.x_velocity = ball.y_velocity = 0;
		end = victory = true;
	};

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	};

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	// I have no idea why the bottom of my console screen is 30 when the window height is supposed to be 40.
	// Playing with the screen maximized resolved this issue and actually displays the paddle as a box instead of just a line with two prongs.
	if (ball.y_position == WINDOW_HEIGHT) {
		ball.x_velocity = ball.y_velocity = 0;
		end = true;
	};
}
