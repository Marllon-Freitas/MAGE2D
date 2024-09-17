#ifndef _MAGE2D_ENGINE_H_
#define _MAGE2D_ENGINE_H_

#include "Game.h"                       // game implementation
#include "Window.h"                     // game window
#include "Graphics.h"                   // graphics hardware
#include "Timer.h"                      // time measurement

class Engine
{
private:
	static Timer m_timer;               // time measurement
	static bool m_isPaused;             // game state

	float CalculateFrameTime();         // calculates the frame time
	int   Loop();                    // starts game execution

public:
	static Game* game;                  // game to be executed
	static Window* window;              // game window
	static Graphics* graphics;          // graphics device
	static float frameTime;             // current frame time

	Engine();
	~Engine();

	int Start(Game* level);             // starts the game execution

	static void Pause();                // pauses the game
	static void Resume();               // resumes the game
};

#endif
