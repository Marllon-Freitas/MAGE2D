#include <windows.h>
#include <sstream>

#include "Engine.h"

Game* Engine::game = nullptr;           // game in execution
Window* Engine::window = nullptr;       // game window
Graphics* Engine::graphics = nullptr;   // graphics device

Engine::Engine()
{
	window = new Window();
	graphics = new Graphics();
}

Engine::~Engine()
{
	delete game;
	delete graphics;
	delete window;
}

int Engine::Start(Game* level)
{
	game = level;

	if (!window->Create())
	{
		MessageBox(nullptr, L"Failed to create window", L"Engine", MB_OK);
		return EXIT_FAILURE;
	}

	// initialize graphics device
	if (!graphics->InitializeDirect3D(window))
	{
		MessageBox(window->GetWindowId(), L"Failed to initialize graphics device", L"Engine", MB_OK);
		return EXIT_FAILURE;
	}

	// return the result of the game execution
	return Loop();
}

int Engine::Loop()
{
	// game initialization
	game->Init();

	// Windows messages
	MSG msg = { 0 };

	// main game loop
	do
	{
		// check if there are Windows messages to process
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// game update
			game->Update();

			// clear the screen for the next frame
			graphics->ClearBackBuffer();

			// draw the game
			game->Draw();

			// present the game on the screen (swap backbuffer/frontbuffer)
			graphics->PresentFrame();

			// FPS control (temporary solution)
			Sleep(16);
		}

	} while (msg.message != WM_QUIT);

	// game finalization
	game->Finalize();

	// exit application
	return int(msg.wParam);
}
