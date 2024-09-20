#include "Object.h"
#include "Engine.h"

Window*& Object::window = Engine::window;           // pointer to the window
float& Object::gameTime = Engine::frameTime;        // time of the last frame

Object::Object() : x(0.0f), y(0.0f), z(0.5f)
{
}

Object::~Object()
{
}
