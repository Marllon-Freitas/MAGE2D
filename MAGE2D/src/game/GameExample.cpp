#include "../game/Resources.h"
#include "../engine/Engine.h"
#include "../engine/Game.h"
#include "../engine/Image.h"
#include "../engine/Sprite.h"

class TestGame : public Game
{
private:
	Sprite* backgroundSprite = nullptr;
	Sprite* knightSprite = nullptr;
	Image* logoImage = nullptr;
	Sprite* logoBehind = nullptr;
	Sprite* logoFront = nullptr;
	float x = 0, y = 0;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

void TestGame::Init()
{
	backgroundSprite = new Sprite("resources/background.jpg");
	knightSprite = new Sprite("resources/knight.png");
	logoImage = new Image("resources/logo.png");
	logoBehind = new Sprite(logoImage);
	logoFront = new Sprite(logoImage);

	x = 80.0f;
	y = 90.0f;
}

void TestGame::Update()
{
	if (m_window->IsKeyDown(VK_ESCAPE))
		m_window->CloseWindow();

	if (m_window->IsKeyDown(VK_LEFT))
		x -= 50.0f * m_gameTime;
	if (m_window->IsKeyDown(VK_RIGHT))
		x += 50.0f * m_gameTime;
	if (m_window->IsKeyDown(VK_UP))
		y -= 50.0f * m_gameTime;
	if (m_window->IsKeyDown(VK_DOWN))
		y += 50.0f * m_gameTime;
}

void TestGame::Draw()
{
	backgroundSprite->Draw(0.0f, 0.0f, Layer::BACK);
	knightSprite->Draw(x, y);
	logoBehind->Draw(40.0f, 60.0f, Layer::UPPER);
	logoFront->Draw(400.0f, 450.0f, Layer::LOWER);
}

void TestGame::Finalize()
{
	// remove sprites from memory
	delete backgroundSprite;
	delete knightSprite;
	delete logoBehind;
	delete logoFront;

	// remove image from memory
	delete logoImage;
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
