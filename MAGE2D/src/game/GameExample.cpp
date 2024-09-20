#include "../game/Resources.h"
#include "../engine/Engine.h"
#include "../engine/Game.h"
#include "../engine/Image.h"
#include "../engine/Sprite.h"

// Game objects
#include "../game/gameObejcts/Knight.h"

class TestGame : public Game
{
private:
	Image* logoImage = nullptr;
	Sprite* backgroundSprite = nullptr;
	Sprite* logoBehind = nullptr;
	Sprite* logoFront = nullptr;

	Knight* knight = nullptr;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

void TestGame::Init()
{
	logoImage = new Image("resources/logo.png");
	logoBehind = new Sprite(logoImage);
	logoFront = new Sprite(logoImage);
	backgroundSprite = new Sprite("resources/background.jpg");

	knight = new Knight();
}

void TestGame::Update()
{
	if (m_window->IsKeyDown(VK_ESCAPE))
		m_window->CloseWindow();

	knight->Update();
}

void TestGame::Draw()
{
	backgroundSprite->Draw(0.0f, 0.0f, Layer::BACK);
	logoBehind->Draw(40.0f, 60.0f, Layer::UPPER);
	logoFront->Draw(400.0f, 450.0f, Layer::LOWER);

	knight->Draw();
}

void TestGame::Finalize()
{
	// remove sprites from memory
	delete backgroundSprite;
	delete logoBehind;
	delete logoFront;

	// remove image from memory
	delete logoImage;

	// remove game objects from memory
	delete knight;
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
	engine->window->SetSize(800, 600);
	engine->window->SetBackgroundColor(30, 30, 30);
	engine->window->SetTitle("Test Game");
	engine->window->SetIcon(IDI_ICON);
	engine->window->SetCursor(NULL);

	//engine->graphics->SetVerticalSync(true);

	// create and start the game
	int status = engine->Start(new TestGame());

	delete engine;
	return status;
}
