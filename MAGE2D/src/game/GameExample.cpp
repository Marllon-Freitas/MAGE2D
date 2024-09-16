#include <random>      // includes random number functions
#include <sstream>

#include "../game/Resources.h"
#include "../engine/Engine.h"
#include "../engine/Game.h"

class WinGame : public Game
{
private:

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

void WinGame::Init()
{

}

void WinGame::Update()
{
	if (window->IsKeyDown(VK_ESCAPE))
		window->CloseWindow();
}

void WinGame::Draw()
{
}

void WinGame::Finalize()
{
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// create engine
	Engine* engine = new Engine();

	// Configure the window
	engine->window->SetMode(WINDOWED);
	engine->window->SetSize(960, 540);
	engine->window->SetBackgroundColor(30, 30, 30);
	engine->window->SetTitle("Window Game");
	engine->window->SetIcon(IDI_ICON);
	engine->window->SetCursor(NULL);

	// create and start the game
	int exit_code = engine->Start(new WinGame());

	delete engine;
	return exit_code;
}
