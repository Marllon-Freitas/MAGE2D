#include "../gameObejcts/Knight.h"

Knight::Knight()
{
	sprite = new Sprite("resources/knight.png");
	MoveTo(80.0f, 90.0f, Layer::MIDDLE);
}

Knight::~Knight()
{
	delete sprite;
}

void Knight::Update()
{
	if (window->IsKeyDown(VK_LEFT))
		x -= 50.0f * gameTime;
	if (window->IsKeyDown(VK_RIGHT))
		x += 50.0f * gameTime;
	if (window->IsKeyDown(VK_UP))
		y -= 50.0f * gameTime;
	if (window->IsKeyDown(VK_DOWN))
		y += 50.0f * gameTime;
}

void Knight::Draw()
{
	sprite->Draw(x, y, z);
}
