#ifndef _MAGE2D_GAME_H_
#define _MAGE2D_GAME_H_

#include "Window.h"

class Game
{
protected:
	static Window*& m_window;                    // game window
	static float& m_gameTime;                    // last frame time

public:
	Game();                                      // constructor
	virtual ~Game();                             // destructor

	// These methods are purely virtual, meaning they must be
	// implemented in all classes derived from Game.

	virtual void OnPause();                      // game pause

	virtual void Init() = 0;                     // game initialization
	virtual void Update() = 0;                   // game update
	virtual void Draw() = 0;                     // scene drawing
	virtual void Finalize() = 0;                 // game finalization
};

#endif
