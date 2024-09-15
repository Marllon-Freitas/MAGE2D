#include <windows.h>
#include <sstream>

#include "Engine.h"

Game* Engine::game = nullptr;       // game in execution
Window* Engine::window = nullptr;   // game window

Engine::Engine()
{
	window = new Window();
}

Engine::~Engine()
{
	delete game;
	delete window;
}

int Engine::Start(Game* level)
{
	game = level;

	// create game window
	window->Create();

	// return the result of the game execution
	return Loop();
}

int Engine::Loop()
{
	MSG  msg = { 0 };   // Windows message
	HDC  hdc;           // device context
	RECT rect;          // client area of the screen

	// capture device context
	hdc = GetDC(window->GetWindowId());

	// get the size of the client area
	GetClientRect(window->GetWindowId(), &rect);

	// game initialization
	game->Init();

	// main game loop
	do
	{
		// handle all events before updating the game
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// game update
			game->Update();

			// clear the client area
			FillRect(hdc, &rect, CreateSolidBrush(window->GetBackgroundColor()));

			// draw the game
			game->Draw();

			Sleep(16);  // FPS control (quick fix)
		}

	} while (msg.message != WM_QUIT);

	// game finalization
	game->Finalize();

	// release device context
	ReleaseDC(window->GetWindowId(), hdc);

	// end application
	return int(msg.wParam);
}
