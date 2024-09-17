#include <random>      // includes random number functions
#include <sstream>

#include "../game/Resources.h"
#include "../engine/Engine.h"
#include "../engine/Game.h"

class TestGame : public Game
{
private:

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

void TestGame::Init()
{

}

void TestGame::Update()
{
	if (m_window->IsKeyDown(VK_ESCAPE))
		m_window->CloseWindow();
}

void TestGame::Draw()
{
}

void TestGame::Finalize()
{
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	Timer timer;
	timer.Start();

	// create engine
	Engine* engine = new Engine();

	// Configure the window
	engine->window->SetMode(WINDOWED);
	engine->window->SetSize(960, 540);
	engine->window->SetBackgroundColor(30, 30, 30);
	engine->window->SetTitle("Test Game");
	engine->window->SetIcon(IDI_ICON);
	engine->window->SetCursor(NULL);

	float timePassed = timer.GetElapsedTime();
	std::wstringstream textTimer;
	textTimer << " -----> Time passed: " << timePassed;

	OutputDebugString(textTimer.str().c_str());

	engine->graphics->SetVerticalSync(true);

	// create and start the game
	int exit_code = engine->Start(new TestGame());

	delete engine;
	return exit_code;
}
