#include "Game.h"
#include "Engine.h"

Window*& Game::m_window = Engine::window;            // pointer to the window
float& Game::m_gameTime = Engine::frameTime;         // time of the last frame

Game::Game()
{
}

Game::~Game()
{
}

void Game::OnPause()
{
	Sleep(10);
}
