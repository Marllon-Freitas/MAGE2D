#include <random>      // includes random number functions
#include <sstream>

#include "../engine/Resources.h"
#include "../engine/Engine.h"
#include "../engine/Game.h"

class WinGame : public Game
{
private:
	HDC hdc;
	int xIni, yIni, xFim, yFim;
	int velXIn, velXFim, velYIn, velYFim;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

void WinGame::Init()
{
	hdc = GetDC(window->GetWindowId());
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 50);
	std::uniform_int_distribution<int> vel(1, 20);

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	velXIn = vel(gen);
	velXFim = vel(gen);
	velYIn = vel(gen);
	velYFim = vel(gen);

	xIni = dis(gen);
	yIni = dis(gen);
	xFim = dis(gen);
	yFim = dis(gen);
}

void WinGame::Update()
{
	if (window->IsKeyDown(VK_ESCAPE))
		window->CloseWindow();

	xIni += velXIn;
	yIni += velYIn;
	xFim += velXFim;
	yFim += velYFim;

	if (xIni < 0 || xIni > window->GetWidth())
		velXIn = -velXIn;

	if (yIni < 0 || yIni > window->GetHeight())
		velYIn = -velYIn;

	if (xFim < 0 || xFim > window->GetWidth())
		velXFim = -velXFim;

	if (yFim < 0 || yFim > window->GetHeight())
		velYFim = -velYFim;

	MoveToEx(hdc, xIni, yIni, NULL);
}

void WinGame::Draw()
{
	LineTo(hdc, xFim, yFim);
}

void WinGame::Finalize()
{
	ReleaseDC(window->GetWindowId(), hdc);
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// create engine and configure the window
	Engine* engine = new Engine();
	engine->window->SetMode(WINDOWED);
	engine->window->SetSize(960, 540);
	engine->window->SetBackgroundColor(30, 30, 30);
	engine->window->SetTitle("Window Game");
	engine->window->SetIcon(IDI_ICON);
	engine->window->SetCursor(NULL);

	// create and start the game
	engine->Start(new WinGame());
}
