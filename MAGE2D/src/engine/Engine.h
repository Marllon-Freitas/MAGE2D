#ifndef _MAGE2D_ENGINE_H_
#define _MAGE2D_ENGINE_H_

#include "Game.h"                       // game implementation
#include "Window.h"                     // game window
#include "Graphics.h"                   // graphics hardware

class Engine
{
private:
	int Loop();                         // starts the real-time loop

public:
	static Game* game;                  // game to be executed
	static Window* window;              // game window
	static Graphics* graphics;          // graphics device

	Engine();
	~Engine();

	int Start(Game* level);             // starts the game execution
};

#endif
