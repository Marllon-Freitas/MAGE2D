#include <windows.h>
#include <sstream>
#include <string>

#include "Engine.h"

Game* Engine::game = nullptr;           // game in execution
Window* Engine::window = nullptr;       // game window
Graphics* Engine::graphics = nullptr;   // graphics device
float Engine::frameTime = 0.0f;         // current frame time
bool Engine::m_isPaused = false;        // game loop state
Timer Engine::m_timer;                  // time measurement

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

	// adjust Sleep function resolution to 1 millisecond
	// requires winmm.lib
	timeBeginPeriod(1);

	int exitCode = Loop();

	// restore original resolution
	timeEndPeriod(1);

	return exitCode;
}

int Engine::Loop()
{
	// start time measurement
	m_timer.Start();

	// game initialization
	game->Init();

	// Windows messages
	MSG msg = { 0 };

	// control for Pause key (VK_CONTROL)
	bool pauseKeyCtrl = true;

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
			// -----------------------------------------------
			// Pause/Resume Game
			// -----------------------------------------------

			if (pauseKeyCtrl)
			{
				if (window->IsKeyDown(VK_CONTROL))
				{
					m_isPaused = !m_isPaused;
					pauseKeyCtrl = false;

					if (m_isPaused)
						m_timer.Stop();
					else
						m_timer.Start();
				}
			}
			else
			{
				// register new key press only after Pause key is released
				if (window->IsKeyUp(VK_CONTROL))
					pauseKeyCtrl = true;
			}

			// -----------------------------------------------

			if (!m_isPaused)
			{
				// calculate frame time
				frameTime = CalculateFrameTime();

				// game update
				game->Update();

				// clear the screen for the next frame
				graphics->ClearBackBuffer();

				// draw the game
				game->Draw();

				// present the game on the screen (swap backbuffer/frontbuffer)
				graphics->PresentFrame();
			}
			else
			{
				// pause screen
				game->OnPause();
			}

			// ----------------------------------------------
		}

	} while (msg.message != WM_QUIT);

	// game finalization
	game->Finalize();

	// exit application
	return int(msg.wParam);
}

float Engine::CalculateFrameTime()
{
	// ----- START DEBUG ----------
#ifdef _DEBUG
	static float totalTime = 0.0f;    // total elapsed time
	static uint frameCount = 0;       // frame count
#endif
	// ------ END DEBUG -----------

	// current frame time in seconds
	frameTime = m_timer.Reset();

	// ----- START DEBUG ----------
#ifdef _DEBUG
	// accumulated frame time
	totalTime += frameTime;

	// increment frame count
	frameCount++;

	// update FPS indicator in the window every 1000ms (1 second)
	if (totalTime >= 1.0f)
	{
		std::wstringstream text;       // text stream for messages
		text << std::fixed;           // always show fractional part
		text.precision(3);            // three decimal places

		text << window->GetTitle().c_str() << "    "
			<< "FPS: " << frameCount << "    "
			<< "Frame Time: " << frameTime * 1000 << " (ms)";

		SetWindowText(window->GetWindowId(), text.str().c_str());

		frameCount = 0;
		totalTime -= 1.0f;
	}
#endif
	// ------ END DEBUG ----------- 

	return frameTime;
}

void Engine::Pause()
{
	m_isPaused = true;
	m_timer.Stop();
}

void Engine::Resume()
{
	m_isPaused = false;
	m_timer.Start();
}
